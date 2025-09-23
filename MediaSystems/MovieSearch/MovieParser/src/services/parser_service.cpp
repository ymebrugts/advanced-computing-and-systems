#include "parser_service.h"
#include "../parsers/movie_parser.h"
#include "../parsers/rating_parser.h"
#include "../parsers/tags_parser.h"

#include <future>
#include <iostream>
#include <utility>

#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

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

    void parser_service::print_progress_top_right(int movies, int tags, int ratings) {
        // Build progress string
        std::ostringstream oss;
        oss << "Movies: " << std::setw(3) << movies << "%"
            << " Tags: " << std::setw(3) << tags << "%"
            << " Ratings: " << std::setw(3) << ratings << "%";
        std::string text = oss.str();

        // Detect terminal width
        int width = 80; // fallback default
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
#else
        struct winsize w {};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
            width = w.ws_col;
        }
#endif

        int col = (width - static_cast<int>(text.size()) + 1);
        if (col < 1) col = 1;

        // Save cursor
        std::cout << "\033[s";

        // Move to row 1, correct col
        std::cout << "\033[1;" << col << "H";

        // Clear to end of line
        std::cout << "\033[K";

        // Print text
        std::cout << text;

        // Restore cursor
        std::cout << "\033[u" << std::flush;
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
                    int m = movies_progress.load();
                    int t = tags_progress.load();
                    int r = ratings_progress.load();

                    print_progress_top_right(m, t, r);

                    if (m == 100 && t == 100 && r == 100) break;
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }
                std::cout << "\nPreloading complete.\n";
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




