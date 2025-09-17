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
        std::optional<int> extractYear(const std::string& title) {
            auto pos1 = title.find_last_of('(');
            auto pos2 = title.find_last_of(')');

            if (pos1 == std::string::npos || pos2 == std::string::npos || pos2 <= pos1 + 1) {
                return std::nullopt; // no valid parentheses
            }

            auto yearStr = title.substr(pos1 + 1, pos2 - pos1 - 1);

            // Early return if not all digits
            if (std::ranges::any_of(yearStr, [](unsigned char c) { return !std::isdigit(c); }))
            {
                return std::nullopt;
            }        

            try {
                return std::stoi(yearStr);
            }
            catch (const std::out_of_range&) {
                return std::nullopt; // number too large
            }
        }
    }


    std::vector<movie_parser::models::Movie> loadMovies(const std::string& filename) {
        std::vector<movie_parser::models::Movie> movies;
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            auto tokens = shared::utils::split(line, "::");
            if (tokens.size() == 3) {
                movie_parser::models::Movie m;
                m.movie_id = std::stoi(tokens[0]);
                m.title = tokens[1];
                m.genres = tokens[2];
                m.year = extractYear(m.title);
                movies.push_back(m);
            }
        }
        return movies;
    }

}

