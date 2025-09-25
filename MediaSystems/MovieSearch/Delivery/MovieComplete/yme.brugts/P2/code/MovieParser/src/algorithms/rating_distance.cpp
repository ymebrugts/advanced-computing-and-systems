/**
 * author Yme Brugts (s4536622)
 * @file rating_distance.cpp
 * @date 2025-09-24
 */


#include "rating_distance.h"
#include <cmath>
#include <numeric>
#include <optional>
#include <unordered_map>
#include "../models/MovieRating.h"
#include <algorithm>

namespace movie_parser::algorithms
{
    std::optional<double> compute_rating_distance(
        const std::vector<models::MovieRating>& user_one_ratings,
        int user_two_id,
        const std::unordered_map<int, std::unordered_map<int, double>>& ratings_by_user_and_movie)
    {
        auto userTwoNestedMapRatingByUserMovie = ratings_by_user_and_movie.find(user_two_id);
        if (userTwoNestedMapRatingByUserMovie == ratings_by_user_and_movie.end()) {
            return std::nullopt;
        }
		const auto& userTwoMap = userTwoNestedMapRatingByUserMovie->second; // movie_id -> rating

        std::vector<double> distances;
        distances.reserve(std::min(user_one_ratings.size(), userTwoMap.size()));
        for (const auto& ratingOne : user_one_ratings) {
            auto it = userTwoMap.find(ratingOne.movie_id);
            if (it != userTwoMap.end()) {
                double distance = std::abs(ratingOne.rating - it->second);
                distances.push_back(distance);
            }
        }

        if (distances.empty()) {
            return std::nullopt;
        }

        double sum = std::accumulate(distances.begin(), distances.end(), 0.0);
        return sum / distances.size();
    }
}
