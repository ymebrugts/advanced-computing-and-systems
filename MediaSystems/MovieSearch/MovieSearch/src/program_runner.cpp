/**
 * author Yme Brugts (s4536622)
 * @file program_runner.cpp
 * @date 2025-09-16
 */


#include "program_runner.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Services/command_service.h"

#include "rating_parser.h"
#include "tags_parser.h"
#include "movie_parser.h"
#include "models/Query.h"
#include "Services/search_service.h"


const std::string HELP_MESSAGE =
    "Available commands:\n"
    "  moviesearch [options]    Prepare a movie search query (parsing only)\n"
    "    --title <keywords>     Title keywords (multi-word allowed)\n"
    "    --year  <YYYY>         Exact release year\n"
    "    --genre <g1,g2,...>    One or more genres\n"
    "    --tag   <t1,t2,...>    One or more tags\n"
    "\n"
    "Notes:\n"
    "You can also use --option=value.\n"
    "\n"
    "Examples:\n"
    "  moviesearch --title Blood --tag Upton\n"
    "  moviesearch --title Las Vegas\n";


void RunProgram(std::istream& in, std::ostream& out, bool interactiveMode) {
    if (interactiveMode) {
        out << HELP_MESSAGE << std::endl;
    }

    std::string line;
    while (true) {
        if (interactiveMode) {
            out << "Enter command: ";
            out.flush();
        }

        if (!std::getline(in, line)) break;
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "parse")
        {
        	auto tags = movie_parser::parsers::loadTags("../Dataset/tags.dat");
        	auto movies = movie_parser::parsers::loadMovies("../Dataset/movies.dat");
        }
        else if (cmd == "moviesearch") {
            auto tokens = moviesearch::services::tokenize_command_line(line);
            if (tokens.empty()) continue;

            auto args = std::vector<std::string>(tokens.begin() + 1, tokens.end());
            auto res = moviesearch::services::parse_moviesearch_tokens(args);
            for (const auto& w : res.warnings) out << "Warning: " << w << "\n";
            if (!res.ok) {
                for (const auto& e : res.errors) out << "Error: " << e << "\n";
                continue;
            }
            auto tags = movie_parser::parsers::loadTags("../Dataset/tags.dat");
            auto movies = movie_parser::parsers::loadMovies("../Dataset/movies.dat");

            auto matches = movie_search::services::searchMovies(res.query, movies, tags);

            for (const auto& m : matches) {
                out << m.movie_id << "::" << m.title << "::" << m.genres << "\n";
            }
        }
        else if (cmd == "printquery") {
            auto tokens = moviesearch::services::tokenize_command_line(line);
            if (tokens.empty()) continue;

            auto args = std::vector<std::string>(tokens.begin() + 1, tokens.end());
            auto res = moviesearch::services::parse_moviesearch_tokens(args);
            for (const auto& w : res.warnings) out << "Warning: " << w << "\n";
            if (!res.ok) {
                for (const auto& e : res.errors) out << "Error: " << e << "\n";
                continue;
            }
            print_query(out, res.query);
		}
        else if (cmd == "printallmovies")
        {
            auto movies = movie_parser::parsers::loadMovies("../Dataset/movies.dat");

            for (const auto& m : movies) {
                out << m.movie_id << "::" << m.title << "::" << m.genres << "\n";
            }
        }
        else if (cmd == "printallmoviestofile")
        {
            auto movies = movie_parser::parsers::loadMovies("../Dataset/movies.dat");

            std::ofstream file("all_movies.txt"); // choose your filename
            if (!file) {
                out << "Error: could not open all_movies.txt for writing\n";
            }
            else {
                for (const auto& m : movies) {
                    file << m.movie_id << "::" << m.title << "::" << m.genres << "\n";
                }
                out << "Wrote " << movies.size() << " movies to all_movies.txt\n";
            }
        }
        else if (cmd == "help") {
            out << HELP_MESSAGE << std::endl;
        }
        else if (cmd == "end") {
            break;
        }
        else {
            out << "Error: Unknown command '" << cmd << "'.\n";
        }
    }
}


void print_query(std::ostream& out, const movie_search::models::Query& q) {
    out << "Parsed movie search (AND semantics):\n";
    out << "  title_keywords : ";
    if (q.title_keywords.empty()) out << "(none)\n";
    else {
        out << "[";
        for (std::size_t i = 0; i < q.title_keywords.size(); ++i) {
            if (i) out << ", ";
            out << "\"" << q.title_keywords[i] << "\"";
        }
        out << "]\n";
    }
    out << "  year           : " << (q.has_year ? std::to_string(q.year) : "(none)") << "\n";
    out << "  genres         : ";
    if (q.genres.empty()) out << "(none)\n";
    else {
        out << "[";
        for (std::size_t i = 0; i < q.genres.size(); ++i) {
            if (i) out << ", ";
            out << q.genres[i];
        }
        out << "]\n";
    }
    out << "  tags           : ";
    if (q.tags.empty()) out << "(none)\n";
    else {
        out << "[";
        for (std::size_t i = 0; i < q.tags.size(); ++i) {
            if (i) out << ", ";
            out << q.tags[i];
        }
        out << "]\n";
    }
    out << "(execution not implemented; parsing only)\n";
}

