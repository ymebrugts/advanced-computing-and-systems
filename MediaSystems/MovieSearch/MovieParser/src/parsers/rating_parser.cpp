#include <fstream>
#include <iostream>

#include "../models/movie_rating.h"
#include "string_utils.h"

namespace movie_parser::parsers
{
    std::vector<models::movie_rating> loadRatings(const std::string& filename) {
        std::vector<models::movie_rating> ratings;
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            auto tokens = shared::utils::split(line, "::");
            if (tokens.size() == 4) {
                models::movie_rating r;
                r.user_id = std::stoi(tokens[0]);
                r.movie_id = std::stoi(tokens[1]);
                r.rating = std::stod(tokens[2]);
                r.timestamp = std::stol(tokens[3]);
                ratings.push_back(r);
            }
        }
        return ratings;
    }
}

