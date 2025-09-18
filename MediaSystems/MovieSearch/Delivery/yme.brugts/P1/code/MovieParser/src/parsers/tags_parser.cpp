#include <string>
#include <vector>
#include <fstream>

#include "string_utils.h"
#include "../models/MovieTag.h"

namespace movie_parser::parsers
{
    std::vector<models::MovieTag> load_tags(const std::string& filename) {
        std::vector<models::MovieTag> tags;
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            auto tokens = shared::utils::split(line, "::");
            if (tokens.size() == 4) {
                models::MovieTag movie_tag;
                movie_tag.user_id = std::stoi(tokens[0]);
                movie_tag.movie_id = std::stoi(tokens[1]);
                movie_tag.tag = tokens[2];
                movie_tag.timestamp = std::stol(tokens[3]);
                tags.push_back(movie_tag);
            }
        }
        return tags;
    }


}

