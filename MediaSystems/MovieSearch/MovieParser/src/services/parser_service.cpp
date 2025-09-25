#include <future>
#include <iostream>
#include <utility>
#include <unordered_map>

#include "parser_service.h"
#include "../parsers/movie_parser.h"
#include "../parsers/rating_parser.h"
#include "../parsers/tags_parser.h"
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
                int movies_index = 0;
                int tags_index = 0;
                int ratings_index = 0;
                while (!stop_reporting) {
                    movies = movies_progress.load();
                    tags = tags_progress.load();
                    ratings = ratings_progress.load();
                    movies_index = movies_index_progress.load();
                    tags_index = tags_index_progress.load();
                    ratings_index = ratings_index_progress.load();

                    utils::print_progress_top_right(movies, tags, ratings, movies_index, tags_index, ratings_index);

                    if (movies == 100 && tags == 100 && ratings == 100 && movies_index == 100 && tags_index == 100 && ratings_index == 100) break;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
                // Show cursor since preloading is finished
                std::cout << "\033[?25h" << std::flush;

            });
        }

        std::thread([this] {
            // block here, but in background thread
            ensure_movies_loaded();
            ensure_tags_loaded();
            ensure_ratings_loaded();

            // build indices
            ensure_movie_index_built();
            ensure_tags_index_built();
            ensure_ratings_index_built();
        }).detach();
        
    }

    void parser_service::ensure_movies_loaded() {
        std::lock_guard<std::mutex> lock(movies_mutex);

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

    void parser_service::ensure_tags_loaded() {
        std::lock_guard<std::mutex> lock(tags_mutex);
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
    void parser_service::ensure_ratings_loaded() {
        std::lock_guard<std::mutex> lock(ratings_mutex);
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

    void parser_service::ensure_movie_index_built() {
        ensure_movies_loaded();

        std::lock_guard<std::mutex> lock(movies_mutex);
        if (!movie_index_built) {
            size_t total = movies.size();
            size_t count = 0;

            for (auto& movie : movies) {
                movie_by_id[movie.movie_id] = &movie;

                if (++count % 1000 == 0) {
                    movies_index_progress.store(static_cast<int>((count * 100) / total));
                }
            }
            movies_index_progress.store(100);
            movie_index_built = true;
        }
    }

    void parser_service::ensure_tags_index_built() {
        ensure_tags_loaded();

        std::lock_guard<std::mutex> lock(tags_mutex);
        if (!tags_index_built) {
            size_t total = tags.size();
            size_t count = 0;

            for (auto& tag : tags) {
                tags_by_movie[tag.movie_id].push_back(tag);
                if (++count % 1000 == 0) {
                    tags_index_progress.store(static_cast<int>((count * 100) / total));
                }
            }
			tags_index_progress.store(100);
			tags_index_built = true;
        }
    }

    void parser_service::ensure_ratings_index_built() {
        ensure_ratings_loaded();

        std::lock_guard<std::mutex> lock(ratings_mutex);
        if (!ratings_index_built) {
            size_t total = ratings.size();
            size_t count = 0;

            for (auto& rating : ratings) {
                ratings_by_movie[rating.movie_id].push_back(rating);
                ratings_all_users[rating.user_id].push_back(rating);
                ratings_by_user_and_movie[rating.user_id][rating.movie_id] = rating.rating;


                if (++count % 1000 == 0) {
                    ratings_index_progress.store(static_cast<int>((count * 100) / total));
                }
            }
            ratings_index_progress.store(100);
            ratings_index_built = true;
        }
    }


    const models::Movie* parser_service::get_movie_by_movie_id(int movie_id) const {
        const_cast<parser_service*>(this)->ensure_movie_index_built();
        auto it = movie_by_id.find(movie_id);
        return (it != movie_by_id.end()) ? it->second : nullptr;
    }

    const std::vector<models::MovieRating>* parser_service::get_ratings_by_movie_id(int movie_id) const {
        const_cast<parser_service*>(this)->ensure_ratings_index_built();
        auto it = ratings_by_movie.find(movie_id);
        return (it != ratings_by_movie.end()) ? &it->second : nullptr;
    }

    const std::vector<models::MovieRating>* parser_service::get_ratings_by_user_id(int user_id) const {
        const_cast<parser_service*>(this)->ensure_ratings_index_built();
        auto it = ratings_all_users.find(user_id);
        return (it != ratings_all_users.end()) ? &it->second : nullptr;
    }

    const std::vector<models::MovieTag>* parser_service::get_tags_by_movie_id(int movie_id) const {
        const_cast<parser_service*>(this)->ensure_tags_index_built();
        auto it = tags_by_movie.find(movie_id);
        return (it != tags_by_movie.end()) ? &it->second : nullptr;
    }

    const std::unordered_map<int, std::vector<models::MovieRating>>& parser_service::get_all_user_ratings_index() const {
        const_cast<parser_service*>(this)->ensure_ratings_index_built();
        return ratings_all_users;
    }

    const std::unordered_map<int, std::unordered_map<int, double>>& parser_service::get_all_user_movie_ratings_index() const {
        const_cast<parser_service*>(this)->ensure_ratings_index_built();
        return ratings_by_user_and_movie;
    }

    const std::unordered_map<int, std::vector<models::MovieRating>>& parser_service::get_all_movie_ratings_index() const {
        const_cast<parser_service*>(this)->ensure_ratings_index_built();
        return ratings_by_movie;
    }

}




