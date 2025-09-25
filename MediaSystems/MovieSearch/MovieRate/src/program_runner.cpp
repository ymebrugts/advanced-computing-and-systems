/**
 * author Yme Brugts (s4536622)
 * @file program_runner.cpp
 * @date 2025-09-23
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
#include <find_all_by_member.h>

#include "cmdline_utils.h"
#include "rating_parser.h"
#include <parser_service.h>
#include <numeric>
#include <unordered_set>
#include <rating_distance.h>
#include <unordered_map>
#include <atomic>
#include "movie_rate.h"
#include "utils/overlap.h"

const std::string HELP_MESSAGE =
"\nAvailable commands:\n"
"  parse                          Load and parse datasets (movies.dat, tags.dat, ratings.dat)\n"
"  MovieRate [UserID] [MovieID]   Predict a rating for an unseen movie using basic collaborative filtering\n"
"  alltofile                      Write all movies, tags, and ratings to text files\n"
"                                  - all_movies.txt\n"
"                                  - all_tags.txt\n"
"                                  - all_ratings.txt\n"
"  help                       Show this help message\n"
"  end                        Exit the program\n";



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
        else if (cmd == "checkoverlap")
        {
	        movie_rate::utils::check_dataset_overlap_second(parserService.get_all_user_ratings_index());
        }
        else if (cmd == "MovieRate")
        {
            auto tokens = shared::utils::tokenize_command_line(input_line);

            auto movieRateQuery = movie_rate::services::parse_rating_distance_line(tokens);
            if (!movieRateQuery) {
                out << "Error: expected exactly one command, user id and movie id.\n";
                continue;
            }

            auto result = movie_parser::algorithms::predict_movie_rate(
                movieRateQuery.value().user_id,
                movieRateQuery.value().movie_id,
                parserService.get_all_user_ratings_index(),
                parserService.get_all_user_movie_ratings_index()
            );

            if (!result.success) {
                out << result.error_message << "\n";
            }
            else if (result.used_pearson) {
                out << "The predicted rating for UserID: " << result.target_user_id
                    << " and MovieID: " << result.target_movie_id
                    << " is " << result.predicted_rating << "\n"
                    << "Calculated using Pearson correlation fallback due to scarcity.\n\n"
					"1. This takes into account all the ratings and users that have seen that same movie and calculate a correlation score\n"
                    "2. Also calculate a weight based on the amount of movies that they have shared to give a weight to the correlation + Bayesian shrinkage to not overwhelm the result by too much weight\n"
					"3. Normalizes ratings by subtracting the user's mean. Meaning people can be inclined to rate higher or lower in general which should not affect correlation\n"
					"4. Negative correlation is also taken into account on top of positive correlation\n"
                    "--> Combine the correlation, weight and normalized rating to calculate the predicted score)\n";
            }
            else if (result.similar_user_id == result.target_user_id) {
                out << "User " << result.target_user_id
                    << " already rated movie " << result.target_movie_id
                    << " with " << result.predicted_rating << "\n";
            }
            else {
                out << "The predicted rating for UserID: " << result.target_user_id
                    << " and MovieID: " << result.target_movie_id
                    << " is " << result.predicted_rating
                    << " based on the most similar user: " << result.similar_user_id
                    << " with a ratingdistance: " << result.distance << "\n";
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




