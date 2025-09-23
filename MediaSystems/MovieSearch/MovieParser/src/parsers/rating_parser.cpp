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


        const MappedFile mmap(filename);
        if (mmap.empty()) {
            throw std::runtime_error("Could not open or map file: " + filename);
        }

        const char* f = mmap.data();
        const char* l = f + mmap.size();

        // Count total lines
        size_t total_lines = 0;
        const char* ptr = f;
        while (ptr && ptr != l) {
            if ((ptr = static_cast<const char*>(memchr(ptr, '\n', l - ptr)))) {
                ++total_lines;
                ++ptr;
            }
        }
    	ratings.reserve(total_lines);

        size_t current_line = 0;
        const char* line_start = f;
        for (const char* it = f; it != l; ++it) {
            if (*it == '\n' || it == l - 1) {
                const char* line_end = (*it == '\n') ? it : (it + 1);
                std::string_view line(line_start, line_end - line_start);

                size_t p1 = line.find("::");
                size_t p2 = line.find("::", p1 + 2);
                size_t p3 = line.find("::", p2 + 2);

                if (p1 != std::string_view::npos &&
                    p2 != std::string_view::npos &&
                    p3 != std::string_view::npos)
                {
                    models::MovieRating movie_rating;
                    std::from_chars(line.data(), line.data() + p1, movie_rating.user_id);
                    std::from_chars(line.data() + p1 + 2, line.data() + p2, movie_rating.movie_id);
					// rating. We only need .5 precision, so we can parse manually
                    {
                        std::string_view rating_sv = line.substr(p2 + 2, p3 - (p2 + 2));
                        int whole = 0;
                        std::from_chars(rating_sv.data(), rating_sv.data() + rating_sv.size(), whole);

                        if (rating_sv.size() > 2 && rating_sv[rating_sv.size() - 2] == '.' && rating_sv.back() == '5') {
                            movie_rating.rating = whole + 0.5;
                        }
                        else {
                            movie_rating.rating = static_cast<double>(whole);
                        }
                    }
                    std::from_chars(line.data() + p3 + 2, line.data() + line.size(), movie_rating.timestamp);
                    ratings.push_back(std::move(movie_rating));
                }

                ++current_line;
                if (total_lines > 0) {
                    progress.store(static_cast<int>(current_line * 100 / total_lines),
                        std::memory_order_relaxed);
                }

                line_start = it + 1; // next line starts after '\n'
            }
        }

        progress.store(100, std::memory_order_relaxed);
        return ratings;
    }
}

