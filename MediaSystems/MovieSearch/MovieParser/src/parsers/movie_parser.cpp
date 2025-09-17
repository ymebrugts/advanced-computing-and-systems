#include <string>
#include <vector>
#include <fstream>

#include "../models/Movie.h"
#include "string_utils.h"

namespace movie_parser::parsers
{
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
                movies.push_back(m);
            }
        }
        return movies;
    }

}

