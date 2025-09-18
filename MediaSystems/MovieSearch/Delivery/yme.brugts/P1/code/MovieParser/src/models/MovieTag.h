#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file MovieTag.h
 * @date 2025-09-17
 */


#include <string>

namespace movie_parser::models {
	struct MovieTag {
	    int user_id;
	    int movie_id;
	    std::string tag;
	    long timestamp;
	};
}
