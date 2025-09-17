#pragma once
#include <string>

namespace movie_parser::models {
	struct movie_tag {
	    int user_id;
	    int movie_id;
	    std::string tag;
	    long timestamp;
	};
}
