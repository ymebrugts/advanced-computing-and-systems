#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file search_service.h
 * @date 2025-09-17
 */

#include "../models/Query.h"

namespace movie_search::services {

    /**
     * @brief Print a parsed movie search query to the given output stream.
     * @param out Output stream to write to
     * @param query Parsed query object containing titles, year, genres, and tags
     */
    void print_query(std::ostream& out, const movie_search::models::Query& query);

}