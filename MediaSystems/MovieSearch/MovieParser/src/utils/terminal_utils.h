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
     * @param movies_index progress 0-100 for map indexes preload
     * @param tags_index progress 0-100 for map indexes preload
     * @param ratings_index progress 0-100 for map indexes preload
     */
    void print_progress_top_right(int movies, int tags, int ratings, int movies_index, int tags_index, int ratings_index);

    void init_progress_row();
}