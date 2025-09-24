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
        for (const auto& r : targetUserRatings) {
            if (r.movie_id == target_movie_id) {
                result.success = true;
                result.similar_user_id = target_user_id;
                result.predicted_rating = r.rating;
                result.distance = 0.0;
                return result;
            }
        }

        double bestDistance = std::numeric_limits<double>::max();
        int mostSimilarUserId = -1;

        // Find most similar user
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
            result.error_message = "No similar user found to predict rating.";
            return result;
        }

        // Predict rating = rating of most similar user for target movie
        const auto& similarUserRatings = ratings_by_user.at(mostSimilarUserId);
        for (const auto& r : similarUserRatings) {
            if (r.movie_id == target_movie_id) {
                result.success = true;
                result.similar_user_id = mostSimilarUserId;
                result.predicted_rating = r.rating;
                result.distance = bestDistance;
                return result;
            }
        }

        // If similar user hasn't rated target movie
        result.error_message = "Most similar user (" + std::to_string(mostSimilarUserId) +
            ") has not rated movie " + std::to_string(target_movie_id) +
            ". Cannot predict.";
        return result;
    }

}
