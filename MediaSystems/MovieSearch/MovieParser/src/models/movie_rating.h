#pragma once

namespace movie_parser::models {
	struct movie_rating {
	    int user_id;
	    int movie_id;
	    double rating;
	    long timestamp;
	};
}
