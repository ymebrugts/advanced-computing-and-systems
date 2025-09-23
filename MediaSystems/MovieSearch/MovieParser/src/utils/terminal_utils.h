#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file terminal_utils.h
 * @date 2025-09-23
 */

namespace movie_parser::utils {

    /**
     * @brief Print parsing progress in the top right of the Linux or Windows terminal
     * @param movies progress 0-100
     * @param tags progress 0-100
     * @param ratings progress 0-100
     */
    void print_progress_top_right(int movies, int tags, int ratings);
}