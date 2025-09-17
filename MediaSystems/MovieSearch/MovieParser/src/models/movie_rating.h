#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file movie_rating.h
 * @date 2025-09-17
 */

namespace movie_parser::models {
	struct movie_rating {
	    int user_id;
	    int movie_id;
	    double rating;
	    long timestamp;
	};
}
