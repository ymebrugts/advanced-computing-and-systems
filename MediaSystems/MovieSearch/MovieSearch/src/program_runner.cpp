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

#include "tags_parser.h"
#include "movie_parser.h"
#include "string_utils.h"
#include "Services/search_service.h"
#include "Services/terminal_service.h"


const std::string HELP_MESSAGE =
	"Available commands:\n"
	"  moviesearch [options]      Prepare and execute a movie search query\n"
	"    --title <keywords>       Title keywords (multi-word allowed)\n"
	"    --year  <YYYY>           Exact release year\n"
	"    --genre <genres>         One or more genres\n"
	"    --tag   <tags>           One or more tags\n"
	"\n"
	"  parse                      Parse datasets (movies.dat, tags.dat)\n"
	"  print [options]            Show parsed query structure without searching\n"
	"  printall                   Print all movies to stdout\n"
	"  alltofile                  Write all movies to all_movies.txt\n"
	"  help                       Show this help message\n"
	"  end                        Exit the program\n"
	"\n"
	"Examples:\n"
	"  moviesearch --title Blood --tag Upton\n"
	"  moviesearch --title Las Vegas\n";


void RunProgram(std::istream& in, std::ostream& out, bool interactive_mode) {
    if (interactive_mode) {
        out << HELP_MESSAGE << '\n';
    }

    std::string input_line;
    while (true) {
        if (interactive_mode) {
            out << "Enter command: ";
            out.flush();
        }

        if (!std::getline(in, input_line)) break;
        if (input_line.empty()) continue;

        std::istringstream iss(input_line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "parse")
        {
        	auto tags = movie_parser::parsers::load_tags("tags.dat");
        	auto movies = movie_parser::parsers::load_movies("movies.dat");
        }
        else if (cmd == "moviesearch") {
            auto tokens = moviesearch::services::tokenize_command_line(input_line);
            if (tokens.empty()) continue;

            auto args = std::vector<std::string>(tokens.begin() + 1, tokens.end());
            auto parse_result = moviesearch::services::parse_moviesearch_line(args);
            for (const auto& w : parse_result.warnings) out << "Warning: " << w << "\n";
            if (!parse_result.ok) {
                for (const auto& e : parse_result.errors) out << "Error: " << e << "\n";
                continue;
            }
            auto tags = movie_parser::parsers::load_tags("tags.dat");
            auto movies = movie_parser::parsers::load_movies("movies.dat");

            auto matches = movie_search::services::search_movies(parse_result.query, movies, tags);

            for (const auto& m : matches) {
                out << m.movie_id << "::" << m.title << "::" << shared::utils::join(m.genres, "|") << "\n";
            }
        }
        else if (cmd == "print") {
            auto tokens = moviesearch::services::tokenize_command_line(input_line);
            if (tokens.empty()) continue;

            auto args = std::vector<std::string>(tokens.begin() + 1, tokens.end());
            auto parse_result = moviesearch::services::parse_moviesearch_line(args);
            for (const auto& w : parse_result.warnings) out << "Warning: " << w << "\n";
            if (!parse_result.ok) {
                for (const auto& e : parse_result.errors) out << "Error: " << e << "\n";
                continue;
            }
            movie_search::services::print_query(out, parse_result.query);
		}
        else if (cmd == "printall")
        {
            auto movies = movie_parser::parsers::load_movies("movies.dat");

            for (const auto& m : movies) {
                out << m.movie_id << "::" << m.title << "::" << shared::utils::join(m.genres, "|") << "\n";
            }
        }
        else if (cmd == "alltofile")
        {
            auto movies = movie_parser::parsers::load_movies("movies.dat");

            std::ofstream file("all_movies.txt"); // choose your filename
            if (!file) {
                out << "Error: could not open all_movies.txt for writing\n";
            }
            else {
                for (const auto& m : movies) {
                    file << m.movie_id << "::" << m.title << "::" << shared::utils::join(m.genres, "|") << "\n";
                }
                out << "Wrote " << movies.size() << " movies to all_movies.txt\n";
            }
        }
        else if (cmd == "help") {
            out << HELP_MESSAGE << '\n';
        }
        else if (cmd == "end") {
            break;
        }
        else {
            out << "Error: Unknown command '" << cmd << "'.\n";
        }
    }
}




