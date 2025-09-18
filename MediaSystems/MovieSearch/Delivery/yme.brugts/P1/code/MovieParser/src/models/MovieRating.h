#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file MovieRating.h
 * @date 2025-09-17
 */

namespace movie_parser::models {
	struct MovieRating {
	    int user_id;
	    int movie_id;
	    double rating;
	    long timestamp;
	};
}
