/**
 * author Yme Brugts (s4536622)
 * @file terminal_service.cpp
 * @date 2025-09-18
 */

#include <ostream>
#include <string>

#include "../models/Query.h"


namespace movie_search::services {
    void print_query(std::ostream& out, const movie_search::models::Query& query) {
        out << "Parsed movie search (AND semantics):\n";
        out << "  titles : ";
        if (query.titles.empty()) out << "(none)\n";
        else {
            out << "[";
            for (std::size_t i = 0; i < query.titles.size(); ++i) {
                if (i) out << ", ";
                out << "\"" << query.titles[i] << "\"";
            }
            out << "]\n";
        }
        out << "  year           : " << (query.has_year ? std::to_string(query.year) : "(none)") << "\n";
        out << "  genres         : ";
        if (query.genres.empty()) out << "(none)\n";
        else {
            out << "[";
            for (std::size_t i = 0; i < query.genres.size(); ++i) {
                if (i) out << ", ";
                out << query.genres[i];
            }
            out << "]\n";
        }
        out << "  tags           : ";
        if (query.tags.empty()) out << "(none)\n";
        else {
            out << "[";
            for (std::size_t i = 0; i < query.tags.size(); ++i) {
                if (i) out << ", ";
                out << query.tags[i];
            }
            out << "]\n";
        }
        out << "(parsing of command only)\n";
    }
}
