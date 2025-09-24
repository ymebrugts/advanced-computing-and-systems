#include "parser_service.h"
#include "../parsers/movie_parser.h"
#include "../parsers/rating_parser.h"
#include "../parsers/tags_parser.h"

#include <future>
#include <iostream>
#include <utility>
#include "../utils/terminal_utils.h"


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
                int movies = 0;
                int tags = 0;
                int ratings = 0;
                while (!stop_reporting) {
                    movies = movies_progress.load();
                    tags = tags_progress.load();
                    ratings = ratings_progress.load();

                    utils::print_progress_top_right(movies, tags, ratings);

                    if (movies == 100 && tags == 100 && ratings == 100) break;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
                // Show cursor since preloading is finished
                std::cout << "\033[?25h" << std::flush;

            });
        }
    }

    void parser_service::ensure_movies_loaded()
    {
        if (!movies_loaded) {
            if (!movies_future.valid()) {
                movies_future = std::async(std::launch::async, [this] {
                    return movie_parser::parsers::load_movies(movies_file, movies_progress);
                    });
            }
            movies = movies_future.get();
            movies_loaded = true;
        }
    }

    void parser_service::ensure_tags_loaded()
    {
        if (!tags_loaded) {
            if (!tags_future.valid()) {
                tags_future = std::async(std::launch::async, [this] {
                    return movie_parser::parsers::load_tags(tags_file, tags_progress);
                    });
            }
            tags = tags_future.get();
            tags_loaded = true;
        }
    }
    void parser_service::ensure_ratings_loaded()
    {
        if (!ratings_loaded) {
            if (!ratings_future.valid()) {
                ratings_future = std::async(std::launch::async, [this] {
                    return movie_parser::parsers::load_ratings(ratings_file, ratings_progress);
                    });
            }
            ratings = ratings_future.get();
            ratings_loaded = true;
        }
    }

    const std::vector<movie_parser::models::Movie>& parser_service::get_movies() {
        ensure_movies_loaded();
        return movies;
    }

    const std::vector<movie_parser::models::MovieTag>& parser_service::get_tags() {
        ensure_tags_loaded();
        return tags;
    }

    const std::vector<movie_parser::models::MovieRating>& parser_service::get_ratings() {
        ensure_ratings_loaded();
        return ratings;
    }

    void parser_service::ensure_movies_index_built() {
        ensure_movies_loaded();
        if (!movie_index_built) {
            for (auto& m : movies) {
                movie_by_id[m.movie_id] = &m;
            }
            movie_index_built = true;
        }
    }

    void parser_service::ensure_ratings_index_built() {
        ensure_ratings_loaded();
        if (!ratings_index_built) {
            for (auto& r : ratings) {
                ratings_by_movie[r.movie_id].push_back(r);
                ratings_by_user[r.user_id].push_back(r);
            }
            ratings_index_built = true;
        }
    }

    void parser_service::ensure_tags_index_built() {
        ensure_tags_loaded();
        if (!tags_index_built) {
            for (auto& t : tags) {
                tags_by_movie[t.movie_id].push_back(t);
            }
            tags_index_built = true;
        }
    }

}




