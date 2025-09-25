/**
 * author Yme Brugts (s4536622)
 * @file movie_rate.cpp
 * @date 2025-09-24
 */

#include "movie_rate.h"
#include "rating_distance.h"
#include <limits>
#include <unordered_set>
#include <cmath>

#include "pearson.h"

namespace movie_parser::algorithms {

    double genre_similarity(const movie_parser::models::Movie& a,
        const movie_parser::models::Movie& b) {
        if (a.genres.empty() || b.genres.empty()) return 0.0;

        std::unordered_set<std::string> ga(a.genres.begin(), a.genres.end());
        int overlap = 0;
        for (const auto& g : b.genres) {
            if (ga.count(g)) overlap++;
        }

        return static_cast<double>(overlap) /
            std::sqrt(static_cast<double>(a.genres.size() * b.genres.size()));
    }


    models::MovieRateResult predict_movie_rate(
        int target_user_id,
        int target_movie_id,
        const std::unordered_map<int, std::vector<movie_parser::models::MovieRating>>& ratings_by_user,
        const std::unordered_map<int, std::unordered_map<int, double>>& ratings_by_user_and_movie,
        const std::unordered_map<int, movie_parser::models::Movie*>& movies_by_id
    )
    {
        models::MovieRateResult result;
        result.target_user_id = target_user_id;
        result.target_movie_id = target_movie_id;

        auto it = ratings_by_user.find(target_user_id);
        if (it == ratings_by_user.end()) {
            result.error_message = "User " + std::to_string(target_user_id) + " has no ratings.";
            return result;
        }
        const auto& targetUserRatings = it->second;

        // Check if user already rated the movie
        auto mapIt = ratings_by_user_and_movie.find(target_user_id); // user_id to movie_id
        if (mapIt != ratings_by_user_and_movie.end()) {
            const auto& movieRatingsMap = mapIt->second; // movie_id to rating
            auto ratingIt = movieRatingsMap.find(target_movie_id);
            if (ratingIt != movieRatingsMap.end()) {
                result.success = false;
                result.similar_user_id = target_user_id;
                result.predicted_rating = ratingIt->second;
                result.distance = 0.0;
                result.error_message = "User already rated this movie.";
                return result;
            }
        }

        double bestDistance = std::numeric_limits<double>::max();
        int mostSimilarUserId = -1;

        // Find most similar user (absolute distance)
        for (const auto& [otherUserId, otherRatings] : ratings_by_user) {
            if (otherUserId == target_user_id) continue;

            auto distance = compute_rating_distance(targetUserRatings, otherUserId, ratings_by_user_and_movie);
            if (!distance) continue;

            if (*distance < bestDistance ||
                (*distance == bestDistance && otherUserId < mostSimilarUserId)) {
                bestDistance = *distance;
                mostSimilarUserId = otherUserId;
            }
        }

        // Predict rating = rating of most similar user for target movie
        auto mostSimilarUserIt = ratings_by_user_and_movie.find(mostSimilarUserId);
        if (mostSimilarUserIt != ratings_by_user_and_movie.end()) {
            const auto& similarUserMovieMap = mostSimilarUserIt->second;
            auto ratingIt = similarUserMovieMap.find(target_movie_id);
            if (ratingIt != similarUserMovieMap.end()) {
                result.success = true;
                result.similar_user_id = mostSimilarUserId;
                result.predicted_rating = ratingIt->second;
                result.distance = bestDistance;
                return result;
            }
        }

        // Pearson correlation fallback
        double weightedSum = 0.0;
        double weightTotal = 0.0;

        // Check all users who rated this movie
        auto mean_of = [](const std::unordered_map<int, double>& m) {
            if (m.empty()) return 0.0;
            double s = 0.0;
            for (const auto& kv : m) s += kv.second;
            return s / static_cast<double>(m.size());
        };

        // target user's mean (add back after combining centered neighbor ratings)
        auto tIt = ratings_by_user_and_movie.find(target_user_id);
        if (tIt == ratings_by_user_and_movie.end() || tIt->second.empty()) {
            result.error_message = "User " + std::to_string(target_user_id) + " has no ratings.";
            return result;
        }
        double targetMean = mean_of(tIt->second);

        // Iterate over all users; keep only those who rated the target movie
        for (const auto& [otherUserId, movieMap] : ratings_by_user_and_movie) {
            if (otherUserId == target_user_id) continue;

            auto itMovie = movieMap.find(target_movie_id);
            if (itMovie == movieMap.end()) continue;  // this user didn't rate the target movie

            const double neighborRatingForMovie = itMovie->second;

            // Pearson(u, v) on co-rated items; min_overlap is enforced inside
            auto pearsonRes = compute_pearson_similarity(
                target_user_id, otherUserId, ratings_by_user_and_movie);

            if (!pearsonRes.valid) continue;

            //Only positive correlations.
            //if (pearsonRes.correlation <= 0.0) continue;

            // Scarcity weighting: correlation * overlap
            // Bayesian/shrinkage significance weighting to avoid overwhelming the result just because someone rated a lot of overlapping items
            const int n = pearsonRes.overlap;
            constexpr int k = 25;
            const double s = static_cast<double>(n) / (n + k);
            const double weight = pearsonRes.correlation * s;

            // Neighbor mean of all their ratings (user-based CF)
            const double neighborMean = mean_of(movieMap);

            // Mean-centered contribution from this neighbor
            weightedSum += weight * (neighborRatingForMovie - neighborMean);
            weightTotal += std::abs(weight);
        }

        // Item based genre CF
        double itemWeightedSum = 0.0;
        double itemWeightTotal = 0.0;

        auto targetMovieIt = movies_by_id.find(target_movie_id);
        if (targetMovieIt != movies_by_id.end()) {
            const auto* targetMovie = targetMovieIt->second;
            if (targetMovie) {
                for (const auto& r : targetUserRatings) {
                    auto neighborMovieIt = movies_by_id.find(r.movie_id);
                    if (neighborMovieIt == movies_by_id.end()) continue;

                    const auto* neighborMovie = neighborMovieIt->second;
                    if (!neighborMovie) continue;

                    double sim = genre_similarity(*targetMovie, *neighborMovie);
                    if (sim <= 0.0) continue;

                    itemWeightedSum += sim * r.rating;
                    itemWeightTotal += sim;
                }
            }
        }

        double finalPrediction = 0.0;
        bool havePearson = (weightTotal > 0.0);
        bool haveItem = (itemWeightTotal > 0.0);

        if (havePearson && haveItem) {
            double pearsonPrediction = targetMean + (weightedSum / weightTotal);
            double itemPrediction = itemWeightedSum / itemWeightTotal;

            constexpr double alpha = 0.7; // weight for Pearson
            constexpr double beta = 1.0 - alpha;

            finalPrediction = alpha * pearsonPrediction + beta * itemPrediction;

            result.success = true;
            result.used_hybrid = true;
            result.similar_user_id = -1;
            result.distance = -1;
            result.predicted_rating = finalPrediction;
            result.error_message.clear();
            return result;
        }
        else if (havePearson) {
            result.success = true;
            result.used_pearson = true;
            result.similar_user_id = -1;
            result.distance = -1;
            result.predicted_rating = targetMean + (weightedSum / weightTotal);
            result.error_message.clear();
            return result;
        }
        else if (haveItem) {
            result.success = true;
            result.used_item_based = true;
            result.similar_user_id = -1;
            result.distance = -1;
            result.predicted_rating = itemWeightedSum / itemWeightTotal;
            result.error_message.clear();
            return result;
        }

        // If still nothing
        result.error_message = "The most similar user (" + std::to_string(mostSimilarUserId) +
            ") has not rated the movie " + std::to_string(target_movie_id) +
            " and Pearson fallback does not have enough overlap. Item-based also not available.";
        return result;
    }

}
