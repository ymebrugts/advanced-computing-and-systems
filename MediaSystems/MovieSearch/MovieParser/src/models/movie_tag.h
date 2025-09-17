#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file movie_tag.h
 * @date 2025-09-17
 */


#include <string>

namespace movie_parser::models {
	struct movie_tag {
	    int user_id;
	    int movie_id;
	    std::string tag;
	    long timestamp;
	};
}
