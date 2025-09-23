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

#include "MovieRating.h"
#include "Services/command_service.h"

#include "tags_parser.h"
#include "movie_parser.h"
#include "string_utils.h"
#include "Services/search_service.h"
#include "Services/terminal_service.h"
#include <find_all_by_member.h>

#include "cmdline_utils.h"
#include "rating_parser.h"
#include "parser_service.h"

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



void run_program(std::istream& in, std::ostream& out, bool interactive_mode) {
    if (interactive_mode) {
        out << HELP_MESSAGE << '\n';
    }

    movie_parser::services::parser_service parserService("movies.dat", "tags.dat", "ratings.dat");
    parserService.preload_all();
    std::string input_line;
    while (true) {
        if (interactive_mode) {
            out << "\n";
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
        	auto tags = parserService.get_tags();
        	auto movies = parserService.get_movies();
        	auto ratings = parserService.get_ratings();
        }
        else if (cmd == "moviesearch") {
            auto tokens = shared::utils::tokenize_command_line(input_line);
            if (tokens.empty()) continue;

            auto args = std::vector<std::string>(tokens.begin() + 1, tokens.end());
            auto parse_result = moviesearch::services::parse_moviesearch_line(args);
            for (const auto& warning : parse_result.warnings) out << "Warning: " << warning << "\n";
            if (!parse_result.ok) {
                for (const auto& e : parse_result.errors) out << "Error: " << e << "\n";
                continue;
            }
            auto tags = parserService.get_tags();
            auto movies = parserService.get_movies();
            auto ratings = parserService.get_ratings();

            auto matches = movie_search::services::search_movies(parse_result.query, movies, tags);

            for (const auto& movie : matches) {

                double ratingSum = 0.0;
                std::size_t count = 0;

                for (const auto& rating : shared::utils::find_all_by_member(ratings, movie.movie_id, &movie_parser::models::MovieRating::movie_id)) {
                    ratingSum += rating.rating;
                    ++count;
                }

                double average = count > 0 ? ratingSum / count : 0.0;

                out << movie.movie_id << "::" << movie.title << "::" << shared::utils::join(movie.genres, "|") << " " << average << "\n";
            }
        }
        else if (cmd == "print") {
            auto tokens = shared::utils::tokenize_command_line(input_line);
            if (tokens.empty()) continue;

            auto args = std::vector<std::string>(tokens.begin() + 1, tokens.end());
            auto parse_result = moviesearch::services::parse_moviesearch_line(args);
            for (const auto& warning : parse_result.warnings) out << "Warning: " << warning << "\n";
            if (!parse_result.ok) {
                for (const auto& e : parse_result.errors) out << "Error: " << e << "\n";
                continue;
            }
            movie_search::services::print_query(out, parse_result.query);
		}
        else if (cmd == "printall")
        {
            auto movies = parserService.get_movies();

            for (const auto& movie : movies) {
                out << movie.movie_id << "::" << movie.title << "::" << shared::utils::join(movie.genres, "|") << "\n";
            }
        }
        else if (cmd == "alltofile")
        {
            auto movies = parserService.get_movies();
            auto tags = parserService.get_tags();
            auto ratings = parserService.get_ratings();
            // Write movies
            {
                std::ofstream movie_file("all_movies.txt");
                if (!movie_file) {
                    out << "Error: could not open all_movies.txt for writing\n";
                }
                else {
                    for (const auto& movie : movies) {
                        movie_file << movie.movie_id << "::" << movie.title << "::"
                            << shared::utils::join(movie.genres, "|") << "\n";
                    }
                    out << "Wrote " << movies.size() << " movies to all_movies.txt\n";
                }
            }

            // Write tags
            {
                std::ofstream tag_file("all_tags.txt");
                if (!tag_file) {
                    out << "Error: could not open all_tags.txt for writing\n";
                }
                else {
                    for (const auto& tag : tags) {
                        tag_file << tag.user_id << "::" << tag.movie_id << "::"
                            << tag.tag << "::" << tag.timestamp << "\n";
                    }
                    out << "Wrote " << tags.size() << " tags to all_tags.txt\n";
                }
            }

            // Write ratings
            {
                std::ofstream rating_file("all_ratings.txt");
                if (!rating_file) {
                    out << "Error: could not open all_ratings.txt for writing\n";
                }
                else {
                    for (const auto& rating : ratings) {
                        rating_file << rating.user_id << "::" << rating.movie_id << "::"
                            << rating.rating << "::" << rating.timestamp << "\n";
                    }
                    out << "Wrote " << movies.size() << " movies to all_ratings.txt\n";
                }
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




