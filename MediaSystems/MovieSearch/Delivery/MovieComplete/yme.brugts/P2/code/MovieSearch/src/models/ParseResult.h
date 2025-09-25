#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file ParseResult.h
 * @date 2025-09-17
 */

#include <string>
#include <vector>

#include "Query.h"


namespace movie_search::models {
    // Result object with errors/warnings; ok == errors.empty()
    struct ParseResult {
        bool ok = false;
        Query query;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
    };
}
