#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file rating_distance.h
 * @date 2025-09-24
 */

#include <optional>
#include <unordered_map>
#include <vector>
#include "../models/MovieRating.h"

namespace movie_parser::algorithms {

    // Computes average rating distance between two users.
	// Returns std::nullopt if no common movies are rated.
    std::optional<double> compute_rating_distance(
        const std::vector<models::MovieRating>& user_one_ratings,
        int user_two_id,
        const std::unordered_map<int, std::unordered_map<int, double>>& ratings_by_user_and_movie);
}
