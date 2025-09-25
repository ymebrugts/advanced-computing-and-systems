#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file Query.h
 * @date 2025-09-24
 */

#include <string>
#include <vector>

namespace movie_rate::models {
    // A structured representation of the parsed query.
    struct Query {
        int user_id;
        int movie_id;
    };
}
