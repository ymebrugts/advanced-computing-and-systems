#pragma once
#include <string>


namespace movie_parser::models {
    struct movie {
        int movieId;
        std::string title;
        std::string genres;  // or split into vector<string>
    };
}
