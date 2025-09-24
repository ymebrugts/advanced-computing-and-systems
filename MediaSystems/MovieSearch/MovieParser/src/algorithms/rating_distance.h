#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file rating_distance.h
 * @date 2025-09-24
 */

#include <optional>
#include <vector>
#include "../models/MovieRating.h"

namespace movie_parser::algorithms {

    // Computes average rating distance between two users.
	// Returns std::nullopt if no common movies are rated.
    std::optional<double> compute_rating_distance(
        const std::vector<models::MovieRating>& user_one_ratings,
        const std::vector<models::MovieRating>& use_two_ratings);
}
