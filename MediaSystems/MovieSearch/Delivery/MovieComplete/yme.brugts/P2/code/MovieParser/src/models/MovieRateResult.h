#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file MovieRateResult.h
 * @date 2025-09-17
 */

#include <string>


namespace movie_parser::models {
    struct MovieRateResult {
        int target_user_id{};
        int target_movie_id{};
        int similar_user_id{ -1 };
        double predicted_rating{ -1 };
        double distance{ -1 };
        bool success{ false };
        bool used_pearson{ false };
        std::string error_message;
    };
}
