#pragma once

#include <future>
#include <vector>

#include "../models/MovieTag.h"
#include "../parsers/movie_parser.h"
#include "../parsers/rating_parser.h"

namespace movie_parser::services {
	class parser_service {
		public:
			parser_service(
				std::string movies_file,
				std::string tags_file,
				std::string ratings_file);

			void preload_all();

		    const std::vector<models::Movie>& get_movies();
		    const std::vector<models::MovieTag>& get_tags();
		    const std::vector<models::MovieRating>& get_ratings();

		private:
			std::string movies_file;
			std::string tags_file;
			std::string ratings_file;

			std::future<std::vector<models::Movie>> movies_future;
			std::future<std::vector<models::MovieTag>> tags_future;
			std::future<std::vector<models::MovieRating>> ratings_future;

			bool movies_loaded = false;
			bool tags_loaded = false;
			bool ratings_loaded = false;

		    std::vector<models::Movie> movies;
		    std::vector<models::MovieTag> tags;
		    std::vector<models::MovieRating> ratings;
	};
} 
