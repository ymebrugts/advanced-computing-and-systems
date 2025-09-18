#include <fstream>
#include <iostream>

#include "../models/MovieRating.h"
#include "string_utils.h"

namespace movie_parser::parsers
{
    std::vector<models::MovieRating> load_ratings(const std::string& filename) {
        std::vector<models::MovieRating> ratings;
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            auto tokens = shared::utils::split(line, "::");
            if (tokens.size() == 4) {
                models::MovieRating movie_rating;
                movie_rating.user_id = std::stoi(tokens[0]);
                movie_rating.movie_id = std::stoi(tokens[1]);
                movie_rating.rating = std::stod(tokens[2]);
                movie_rating.timestamp = std::stol(tokens[3]);
                ratings.push_back(movie_rating);
            }
        }
        return ratings;
    }
}

