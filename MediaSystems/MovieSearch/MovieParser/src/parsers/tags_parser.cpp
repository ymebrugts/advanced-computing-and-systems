#include <string>
#include <vector>
#include <fstream>

#include "string_utils.h"
#include "../models/MovieTag.h"

namespace movie_parser::parsers
{
    std::vector<models::MovieTag> load_tags(const std::string& filename, std::atomic<int>& progress) {
        std::vector<models::MovieTag> tags;
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
                models::MovieTag movie_tag;
                movie_tag.user_id = std::stoi(tokens[0]);
                movie_tag.movie_id = std::stoi(tokens[1]);
                movie_tag.tag = tokens[2];
                movie_tag.timestamp = std::stol(tokens[3]);
                tags.push_back(movie_tag);
            }

            ++current_line;
            if (total_lines > 0) {
                progress.store(static_cast<int>(current_line * 100 / total_lines),
                    std::memory_order_relaxed);
            }
        }
        progress.store(100, std::memory_order_relaxed);
        return tags;
    }


}

