#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file Query.h
 * @date 2025-09-23
 */

#include <string>
#include <vector>

namespace rating_distance::models {
    // A structured representation of the parsed query.
    struct Query {
        int user_id_one;
		int user_id_two;
    };
}
