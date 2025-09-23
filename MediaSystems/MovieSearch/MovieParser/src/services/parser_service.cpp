#include "parser_service.h"
#include "../parsers/movie_parser.h"
#include "../parsers/rating_parser.h"
#include "../parsers/tags_parser.h"

#include <future>
#include <iostream>
#include <utility>

namespace movie_parser::services {

    parser_service::parser_service(std::string movies_file,
        std::string tags_file,
        std::string ratings_file)
        : movies_file(std::move(movies_file)),
        tags_file(std::move(tags_file)),
        ratings_file(std::move(ratings_file)) {
    }

    parser_service::~parser_service() {
        stop_reporting = true;
        if (reporter.joinable()) {
            reporter.join();
        }
    }

    void parser_service::preload_all() {
        if (!movies_future.valid())
        {
            movies_future = std::async(std::launch::async, [this] {
                return movie_parser::parsers::load_movies(movies_file, movies_progress);
            });
        }


        if (!tags_future.valid())
        {
            tags_future = std::async(std::launch::async, [this] {
				return movie_parser::parsers::load_tags(tags_file, tags_progress);
            });
        }


        if (!ratings_future.valid())
        {
            ratings_future = std::async(std::launch::async, [this] {
                return movie_parser::parsers::load_ratings(ratings_file, ratings_progress);
            });
        }

        // Start reporter only once
        if (!reporter.joinable()) {
            reporter = std::thread([this] {
                while (!stop_reporting) {
                    int movies = movies_progress.load();
                    int tags = tags_progress.load();
                    int ratings = ratings_progress.load();

                    print_progress_top_right(movies, tags, ratings);

                    if (movies == 100 && tags == 100 && ratings == 100) break;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            });
        }
    }

    const std::vector<movie_parser::models::Movie>& parser_service::get_movies() {
        if (!movies_loaded) {
            if (!movies_future.valid()) {
                movies_future = std::async(std::launch::async, [this] {
					return movie_parser::parsers::load_movies(movies_file, movies_progress);
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
                    return movie_parser::parsers::load_tags(tags_file, tags_progress);
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
                    return movie_parser::parsers::load_ratings(ratings_file, ratings_progress);
                });
            }
            ratings = ratings_future.get();
            ratings_loaded = true;
        }
        return ratings;
    }


}




