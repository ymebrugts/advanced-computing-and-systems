#include <fstream>
#include <iostream>

#include "../models/MovieRating.h"
#include "string_utils.h"
#include "mapped_file.hpp"
#include <cstring>
#include <charconv>

namespace movie_parser::parsers
{
    std::vector<models::MovieRating> load_ratings(const std::string& filename, std::atomic<int>& progress) {
        std::vector<models::MovieRating> ratings;

        const MappedFile mapped_file(filename);
        if (mapped_file.empty()) {
            throw std::runtime_error("Could not open or map file: " + filename);
        }

        const char* file_begin = mapped_file.data();
        const char* file_end = file_begin + mapped_file.size();

        // Count total lines
        size_t total_lines = 0;
        const char* newline_search_ptr = file_begin;
        while (newline_search_ptr && newline_search_ptr != file_end) {
            if ((newline_search_ptr = static_cast<const char*>(memchr(newline_search_ptr, '\n', file_end - newline_search_ptr)))) {
                ++total_lines;
                ++newline_search_ptr;
            }
        }
        ratings.reserve(total_lines);

        size_t current_line = 0;
        const char* line_start_ptr = file_begin;
        for (const char* char_ptr = file_begin; char_ptr != file_end; ++char_ptr) {
            if (*char_ptr == '\n' || char_ptr == file_end - 1) {
                const char* line_end_ptr = (*char_ptr == '\n') ? char_ptr : (char_ptr + 1);
                std::string_view line(line_start_ptr, line_end_ptr - line_start_ptr);

                size_t positionDelimiterOne = line.find("::");
                size_t positionDelimiterTwo = line.find("::", positionDelimiterOne + 2);
                size_t positionDelimiterThree = line.find("::", positionDelimiterTwo + 2);

                if (positionDelimiterOne != std::string_view::npos &&
                    positionDelimiterTwo != std::string_view::npos &&
                    positionDelimiterThree != std::string_view::npos)
                {
                    models::MovieRating movie_rating;
                    std::from_chars(line.data(), line.data() + positionDelimiterOne, movie_rating.user_id);
                    std::from_chars(line.data() + positionDelimiterOne + 2, line.data() + positionDelimiterTwo, movie_rating.movie_id);
                    // rating. We only need .5 precision, so we can parse manually
                    {
                        std::string_view rating_sv = line.substr(positionDelimiterTwo + 2, positionDelimiterThree - (positionDelimiterTwo + 2));
                        int whole = 0;
                        std::from_chars(rating_sv.data(), rating_sv.data() + rating_sv.size(), whole);

                        if (rating_sv.size() > 2 &&
                            rating_sv[rating_sv.size() - 2] == '.' &&
                            rating_sv.back() == '5') {
                            movie_rating.rating = whole + 0.5;
                        }
                        else {
                            movie_rating.rating = static_cast<double>(whole);
                        }
                    }
                    std::from_chars(line.data() + positionDelimiterThree + 2, line.data() + line.size(), movie_rating.timestamp);
                    ratings.push_back(movie_rating);
                }

                ++current_line;
                if (total_lines > 0) {
                    progress.store(static_cast<int>(current_line * 100 / total_lines),
                        std::memory_order_relaxed);
                }

                line_start_ptr = char_ptr + 1; // next line starts after '\n'
            }
        }

        progress.store(100, std::memory_order_relaxed);
        return ratings;
    }
}
