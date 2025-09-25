/**
 * author Yme Brugts (s4536622)
 * @file movie_rate.cpp
 * @date 2025-09-24
 */

 // movie_parser/algorithms/movie_rate.cpp
#include "movie_rate.h"
#include "rating_distance.h"
#include <limits>
#include <unordered_set>
#include <cmath>

#include "pearson.h"

namespace movie_parser::algorithms {

    models::MovieRateResult predict_movie_rate(
        int target_user_id,
        int target_movie_id,
        const std::unordered_map<int, std::vector<movie_parser::models::MovieRating>>& ratings_by_user,
        const std::unordered_map<int, std::unordered_map<int, double>>& ratings_by_user_and_movie)
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

        // Iterate over ALL users; keep only those who rated the target movie
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
            const double weight = pearsonRes.correlation * static_cast<double>(pearsonRes.overlap);

            // Neighbor mean of all their ratings (user-based CF)
            const double neighborMean = mean_of(movieMap);

            // Mean-centered contribution from this neighbor
            weightedSum += weight * (neighborRatingForMovie - neighborMean);
            weightTotal += std::abs(weight);
        }

        if (weightTotal > 0.0) {
            result.success = true;
            result.used_pearson = true;
            result.similar_user_id = -1;
            result.distance = -1;
            result.predicted_rating = targetMean + (weightedSum / weightTotal);
            result.error_message.clear();
            return result;
        }

        // If still nothing
        result.error_message = "The most similar user (" + std::to_string(mostSimilarUserId) +
            ") has not rated the movie " + std::to_string(target_movie_id) +
            " and Pearson fallback does not have enough overlap.";
        return result;
    }

}
