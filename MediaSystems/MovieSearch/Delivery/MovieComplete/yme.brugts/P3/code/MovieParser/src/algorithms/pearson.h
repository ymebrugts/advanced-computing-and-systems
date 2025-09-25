#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file pearson.h
 * @date 2025-09-25
 */

#include <unordered_map>
#include "../models/PearsonResult.h"

namespace movie_parser::algorithms {

    PearsonResult compute_pearson_similarity(
        int userOneId,
        int userTwoId,
        const std::unordered_map<int, std::unordered_map<int, double>>& ratings_by_user_and_movie,
        int min_overlap = 5);
}
