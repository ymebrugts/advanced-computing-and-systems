/**
* author Yme Brugts (s4536622)
 * @file container_utils.h
 * @date 2025-01-16
 */

#pragma once
/**
 * @file container_utils.h
 * @brief Container manipulation utilities
 */

#include <string>
#include <vector>

namespace moviesearch {
namespace utils {

    /**
     * @brief Split a string on commas while preserving non-empty parts
     * @param s String to split
     * @return Vector of trimmed, non-empty parts
     */
    std::vector<std::string> split_commas_preserve(const std::string& s);

    /**
     * @brief Remove duplicates from a sequence while preserving order
     * @tparam Seq Sequence type (must support std::string elements)
     * @param v Sequence to deduplicate
     */
    template <class Seq>
    void dedupe_preserve_order(Seq& v);

} // namespace utils
} // namespace moviesearch

// Template implementation
#include "container_utils_impl.h"