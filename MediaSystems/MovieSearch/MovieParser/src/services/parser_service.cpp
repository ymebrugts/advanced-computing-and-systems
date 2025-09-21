#include "parser_service.h"
#include "../parsers/movie_parser.h"
#include "../parsers/rating_parser.h"
#include "../parsers/tags_parser.h"

#include <future>
#include <utility>

namespace movie_parser::services {

    parser_service::parser_service(std::string movies_file,
        std::string tags_file,
        std::string ratings_file)
        : movies_file(std::move(movies_file)),
        tags_file(std::move(tags_file)),
        ratings_file(std::move(ratings_file)) {
    }

    void parser_service::preload_all() {
        if (!movies_future.valid())
        {
            movies_future = std::async(std::launch::async, [this] {
                return movie_parser::parsers::load_movies(movies_file);
            });
        }


        if (!tags_future.valid())
        {
            tags_future = std::async(std::launch::async, [this] {
				return movie_parser::parsers::load_tags(tags_file);
            });
        }


        if (!ratings_future.valid())
        {
            ratings_future = std::async(std::launch::async, [this] {
                return movie_parser::parsers::load_ratings(ratings_file);
            });
        }

    }

    const std::vector<movie_parser::models::Movie>& parser_service::get_movies() {
        if (!movies_loaded) {
            if (!movies_future.valid()) {
                movies_future = std::async(std::launch::async, [this] {
                    return movie_parser::parsers::load_movies(movies_file);
                    });
            }
            movies = movies_future.get();
            movies_loaded = true;
        }
        return movies;
    }

    const std::vector<movie_parser::models::MovieTag>& parser_service::get_tags() {
        if (!tags_loaded) {
            if (!tags_future.valid()) {
                tags_future = std::async(std::launch::async, [this] {
                    return movie_parser::parsers::load_tags(tags_file);
                    });
            }
            tags = tags_future.get();
            tags_loaded = true;
        }
        return tags;
    }

    const std::vector<movie_parser::models::MovieRating>& parser_service::get_ratings() {
        if (!ratings_loaded) {
            if (!ratings_future.valid()) {
                ratings_future = std::async(std::launch::async, [this] {
                    return movie_parser::parsers::load_ratings(ratings_file);
                    });
            }
            ratings = ratings_future.get();
            ratings_loaded = true;
        }
        return ratings;
    }

}
