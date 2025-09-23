#include <fstream>
#include <iostream>

#include "../models/MovieRating.h"
#include "string_utils.h"

namespace movie_parser::parsers
{
    std::vector<models::MovieRating> load_ratings(const std::string& filename, std::atomic<int>& progress) {
        std::vector<models::MovieRating> ratings;
        std::ifstream file(filename);
        std::string line;

        file.unsetf(std::ios_base::skipws);
        size_t total_lines = std::count(std::istream_iterator<char>(file), std::istream_iterator<char>(), '\n');
        file.clear();
        file.seekg(0, std::ios::beg);
        file.setf(std::ios_base::skipws);

        size_t current_line = 0;
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

            ++current_line;
            if (total_lines > 0) {
                progress.store(static_cast<int>(current_line * 100 / total_lines),
                    std::memory_order_relaxed);
            }
        }
        progress.store(100, std::memory_order_relaxed);
        return ratings;
    }
}

