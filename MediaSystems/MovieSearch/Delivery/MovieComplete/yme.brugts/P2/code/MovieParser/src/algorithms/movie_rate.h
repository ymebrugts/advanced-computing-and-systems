#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file movie_rate.h
 * @date 2025-09-24
 */


#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../models/MovieRating.h"
#include "../models/MovieRateResult.h"

namespace movie_parser::algorithms {

	models::MovieRateResult predict_movie_rate(
        int targetUserId,
        int targetMovieId,
        const std::unordered_map<int, std::vector<movie_parser::models::MovieRating>>& ratingsByUser,
        const std::unordered_map<int, std::unordered_map<int, double>>& ratings_by_user_and_movie);


}
