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

        if (mostSimilarUserId == -1) {
            result.error_message = "No similar user were found to predict rating.";
            return result;
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
        auto movieIt = ratings_by_user_and_movie.find(target_movie_id);
        if (movieIt != ratings_by_user_and_movie.end()) {
            for (const auto& [otherUserId, rating] : movieIt->second) {
                if (otherUserId == target_user_id) continue;

                auto pearsonRes = compute_pearson_similarity(
                    target_user_id, otherUserId, ratings_by_user_and_movie);

                if (pearsonRes.valid && pearsonRes.correlation > 0) {
                    // Weight = correlation * overlap
                    double weight = pearsonRes.correlation * pearsonRes.overlap;
                    weightedSum += weight * rating;
                    weightTotal += weight;
                }
            }
        }

        if (weightTotal > 0) {
            result.success = true;
            result.predicted_rating = weightedSum / weightTotal;
            result.similar_user_id = -1;
            result.distance = -1; 
            result.used_pearson = true;  
            result.error_message = "";
            return result;
        }

        // If still nothing
        result.error_message = "The most similar user (" + std::to_string(mostSimilarUserId) +
            ") has not rated the movie " + std::to_string(target_movie_id) +
            " and Pearson fallback does not have enough overlap.";
        return result;
    }

}
