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
                models::MovieTag t;
                t.user_id = std::stoi(tokens[0]);
                t.movie_id = std::stoi(tokens[1]);
                t.tag = tokens[2];
                t.timestamp = std::stol(tokens[3]);
                tags.push_back(t);
            }
        }
        return tags;
    }


}

