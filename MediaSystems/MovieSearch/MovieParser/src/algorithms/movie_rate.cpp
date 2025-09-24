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

    std::optional<models::MovieRateResult> predict_movie_rate(
        int target_user_id,
        int target_movie_id,
        const std::unordered_map<int, std::vector<movie_parser::models::MovieRating>>& ratings_by_user)
    {
        auto it = ratings_by_user.find(target_user_id);
        if (it == ratings_by_user.end()) {
            return std::nullopt; // target user has no ratings
        }
        const auto& targetUserRatings = it->second;

        // Check if user already rated the movie
        for (const auto& r : targetUserRatings) {
            if (r.movie_id == target_movie_id) {
                // already rated → nothing to predict
                return models::MovieRateResult{ target_user_id, target_movie_id, target_user_id, r.rating, 0.0 };
            }
        }

        double bestDistance = std::numeric_limits<double>::max();
        int mostSimilarUserId = -1;

        // Find most similar user
        for (const auto& [otherUserId, otherRatings] : ratings_by_user) {
            if (otherUserId == target_user_id) continue;

            auto distance = compute_rating_distance(targetUserRatings, otherRatings);
            if (!distance) continue;

            if (*distance < bestDistance ||
                (*distance == bestDistance && otherUserId < mostSimilarUserId)) {
                bestDistance = *distance;
                mostSimilarUserId = otherUserId;
            }
        }

        if (mostSimilarUserId == -1) {
            return std::nullopt; // no similar user found
        }

        // Predict rating = rating of most similar user for target movie
        const auto& similarUserRatings = ratings_by_user.at(mostSimilarUserId);
        for (const auto& r : similarUserRatings) {
            if (r.movie_id == target_movie_id) {
                return models::MovieRateResult{
                    target_user_id,
                    target_movie_id,
                    mostSimilarUserId,
                    r.rating,
                    bestDistance
                };
            }
        }

        // If similar user hasn't rated target movie then cannot predict
        return std::nullopt;
    }

}
