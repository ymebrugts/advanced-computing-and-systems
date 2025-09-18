#include <algorithm>
#include <string>
#include <vector>
#include <fstream>

#include "../models/Movie.h"
#include "string_utils.h"
#include <ranges>

namespace movie_parser::parsers
{
    namespace
    {
        std::optional<int> extract_year(const std::string& title) {
            const auto last_left_parentheses = title.find_last_of('(');
            const auto last_right_parentheses = title.find_last_of(')');

            if (last_left_parentheses == std::string::npos || last_right_parentheses == std::string::npos || last_right_parentheses <= last_left_parentheses + 1) {
                return std::nullopt; // no valid parentheses
            }

            auto year_str = title.substr(last_left_parentheses + 1, last_right_parentheses - last_left_parentheses - 1);

            // Early return if not all digits
            if (std::ranges::any_of(year_str, [](const unsigned char c) { return !std::isdigit(c); }))
            {
                return std::nullopt;
            }        

            try {
                return std::stoi(year_str);
            }
            catch (const std::out_of_range&) {
                return std::nullopt; // number too large
            }
        }
    }


    std::vector<models::Movie> load_movies(const std::string& filename) {
        std::vector<models::Movie> movies;
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            auto tokens = shared::utils::split(line, "::");
            if (tokens.size() == 3) {
                movie_parser::models::Movie movie;
                movie.movie_id = std::stoi(tokens[0]);
                movie.title = tokens[1];
                movie.genres = shared::utils::split(tokens[2], "|");
                movie.year = extract_year(movie.title);
                movies.push_back(movie);
            }
        }
        return movies;
    }

}

