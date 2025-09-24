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
        const std::vector<models::MovieRating>& userOneRatings,
        const std::vector<models::MovieRating>& userTwoRatings)
    {
        std::unordered_map<int, double> userTwoMap;
        userTwoMap.reserve(userTwoRatings.size());
        for (const auto& rating : userTwoRatings) {
            userTwoMap[rating.movie_id] = rating.rating;
        }

        std::vector<double> distances;
        distances.reserve(std::min(userOneRatings.size(), userTwoRatings.size()));
        for (const auto& ratingOne : userOneRatings) {
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
