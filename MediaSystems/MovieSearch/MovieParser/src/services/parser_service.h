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
			~parser_service();

			void preload_all();

		    const std::vector<models::Movie>& get_movies();
		    const std::vector<models::MovieTag>& get_tags();
		    const std::vector<models::MovieRating>& get_ratings();

			const models::Movie* get_movie_by_movie_id(int movie_id) const;
			const std::vector<models::MovieRating>* get_ratings_by_movie_id(int movie_id) const;
			const std::vector<models::MovieRating>* get_ratings_by_user_id(int user_id) const;
			const std::vector<models::MovieTag>* get_tags_by_movie_id(int movie_id) const;


		private:
			std::string movies_file;
			std::string tags_file;
			std::string ratings_file;

			std::future<std::vector<models::Movie>> movies_future;
			std::future<std::vector<models::MovieTag>> tags_future;
			std::future<std::vector<models::MovieRating>> ratings_future;

			std::atomic<int> movies_progress{ 0 };
			std::atomic<int> tags_progress{ 0 };
			std::atomic<int> ratings_progress{ 0 };

			std::unordered_map<int, models::Movie*> movie_by_id;
			std::unordered_map<int, std::vector<models::MovieRating>> ratings_by_movie;
			std::unordered_map<int, std::vector<models::MovieRating>> ratings_by_user;
			std::unordered_map<int, std::vector<models::MovieTag>> tags_by_movie;

			bool movies_loaded = false;
			bool tags_loaded = false;
			bool ratings_loaded = false;

			bool movie_index_built = false;
			bool ratings_index_built = false;
			bool tags_index_built = false;

		    std::vector<models::Movie> movies;
		    std::vector<models::MovieTag> tags;
		    std::vector<models::MovieRating> ratings;

			std::atomic<bool> stop_reporting{ false };
			std::thread reporter;

			void ensure_movies_loaded();
			void ensure_tags_loaded();
			void ensure_ratings_loaded();

			void ensure_movie_index_built();
			void ensure_tags_index_built();
			void ensure_ratings_index_built();

			mutable std::mutex movies_mutex;
			mutable std::mutex tags_mutex;
			mutable std::mutex ratings_mutex;

	};
} 
