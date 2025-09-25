/**
 * author Yme Brugts (s4536622)
 * @file overlap.cpp
 * @date 2025-09-25
 */


#include <set>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "MovieRating.h"
namespace movie_rate::utils {

    void check_dataset_overlap(
        const std::unordered_map<int, std::vector<movie_parser::models::MovieRating>>& ratings_by_movie,
        int threshold = 5)
    {
        std::unordered_map<long long, int> overlapCounts;
        overlapCounts.reserve(1000000);

        auto makeKey = [](int a, int b) -> long long {
            if (a > b) std::swap(a, b);
            return (static_cast<long long>(a) << 32) | static_cast<unsigned int>(b);
            };

        size_t totalMovies = ratings_by_movie.size();
        size_t processed = 0;

        long long validPairs = 0;

        for (const auto& [movieId, ratings] : ratings_by_movie) {
            for (size_t i = 0; i < ratings.size(); i++) {
                for (size_t j = i + 1; j < ratings.size(); j++) {
                    long long key = makeKey(ratings[i].user_id, ratings[j].user_id);
                    int& count = overlapCounts[key];
                    count++;

                    if (count == threshold) {
                        validPairs++;
                    }
                                        
                    if (count >= threshold) {
                        overlapCounts.erase(key);
                    }
                }
            }

            if (++processed % 1 == 0) {
                int percent = static_cast<int>((processed * 100) / totalMovies);
                std::cout << "\rProcessing movies: " << processed << "/" << totalMovies
                    << " (" << percent << "%)" << std::flush;
            }
        }

        std::cout << "\n";
        std::cout << "Total user pairs that ever overlapped (tracked partially): " << (validPairs + overlapCounts.size()) << "\n";
        std::cout << "Pairs with at least " << threshold << " overlaps: " << validPairs << "\n";
    }

    void check_dataset_overlap_second(
        const std::unordered_map<int, std::vector<movie_parser::models::MovieRating>>& ratings_by_user,
        int threshold = 5)
    {
        int totalPairs = 0;
        int validPairs = 0;

        size_t totalUsers = ratings_by_user.size();
        size_t processedUsers = 0;

        for (auto it1 = ratings_by_user.begin(); it1 != ratings_by_user.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != ratings_by_user.end(); ++it2) {
                // Build sets of movie IDs
                std::set<int> movies1, movies2;
                for (const auto& r : it1->second) movies1.insert(r.movie_id);
                for (const auto& r : it2->second) movies2.insert(r.movie_id);

                // Count intersection
                int overlap = 0;
                for (int m : movies1) {
                    if (movies2.count(m)) overlap++;
                }

                totalPairs++;
                if (overlap >= threshold) {
                    validPairs++;
                }
            }

            processedUsers++;

            // Print progress every 50 users
            if (processedUsers % 1 == 0 || processedUsers == totalUsers) {
                int percent = static_cast<int>((processedUsers * 100) / totalUsers);
                std::cout << "\rProcessing users: " << processedUsers << "/" << totalUsers
                    << " (" << percent << "%)" << std::flush;
            }
        }

        std::cout << "\n"; // move to next line after progress

        std::cout << "Total user pairs: " << totalPairs << "\n";
        std::cout << "Pairs with at least " << threshold << " overlaps: " << validPairs
            << " (" << (100.0 * validPairs / totalPairs) << "%)\n";
    }

}
