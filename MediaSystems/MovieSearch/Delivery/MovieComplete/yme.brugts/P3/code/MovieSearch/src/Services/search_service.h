#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file search_service.h
 * @date 2025-09-17
 */

#include <vector>
#include "Movie.h"
#include "MovieTag.h"
#include "../models/Query.h"

namespace movie_search::services {

    /**
     * @brief Search movies based on a parsed query
     *
     * @param The query (title keywords, year, genres, tags)
     * @param movies Parsed movies from movies.dat
     * @param tags Parsed tags from tags.dat
     * @return Vector of matching movies
     */
    std::vector<movie_parser::models::Movie> search_movies(
        const movie_search::models::Query&,
        const std::vector<movie_parser::models::Movie>& movies,
        const std::vector<movie_parser::models::MovieTag>& tags
    );

}