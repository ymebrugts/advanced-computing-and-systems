/**
 * author Yme Brugts (s4536622)
 * @file pearson.cpp
 * @date 2025-09-24
 */

 // movie_parser/algorithms/movie_rate.cpp
#include "movie_rate.h"
#include "rating_distance.h"
#include <limits>
#include <unordered_set>
#include "../models/PearsonResult.h"

namespace movie_parser::algorithms {
    PearsonResult compute_pearson_similarity(
        int userOneId,
        int userTwoId,
        const std::unordered_map<int, std::unordered_map<int, double>>& ratings_by_user_and_movie,
        int min_overlap = 5)
    {
        auto it1 = ratings_by_user_and_movie.find(userOneId);
        auto it2 = ratings_by_user_and_movie.find(userTwoId);
        if (it1 == ratings_by_user_and_movie.end() || it2 == ratings_by_user_and_movie.end()) {
            return {};
        }

        const auto& ratings1 = it1->second; // movie_id -> rating
        const auto& ratings2 = it2->second;

        double sum1 = 0.0, sum2 = 0.0;
        double sum1Sq = 0.0, sum2Sq = 0.0;
        double pSum = 0.0;
        int overlap = 0;

        const auto& smaller = (ratings1.size() < ratings2.size()) ? ratings1 : ratings2;
        const auto& larger = (ratings1.size() < ratings2.size()) ? ratings2 : ratings1;

        for (const auto& [movieId, rating1] : smaller) {
            auto it = larger.find(movieId);
            if (it != larger.end()) {
                double rating2 = it->second;
                sum1 += rating1;
                sum2 += rating2;
                sum1Sq += rating1 * rating1;
                sum2Sq += rating2 * rating2;
                pSum += rating1 * rating2;
                overlap++;
            }
        }

        if (overlap < min_overlap) {
            return {}; // not valid
        }

        double num = pSum - (sum1 * sum2 / overlap);
        double den = std::sqrt((sum1Sq - (sum1 * sum1 / overlap)) *
            (sum2Sq - (sum2 * sum2 / overlap)));

        if (den == 0) return {};

        return { num / den, overlap, true };
    }



}
