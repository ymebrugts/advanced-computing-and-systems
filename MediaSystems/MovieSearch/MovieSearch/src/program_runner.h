#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file program_runner.h
 * @date 2025-09-16
 */


#include <ostream>
#include "models/Query.h"

void RunProgram(std::istream& in, std::ostream& out, bool interactive_mode);

void print_query(std::ostream& out, const movie_search::models::Query& query);
