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

namespace shared::utils {
    /**
     * @brief Remove duplicates from a sequence while preserving order
     * @tparam Seq Sequence type (must support std::string elements)
     * @param v Sequence to deduplicate
     */
    template <class Seq>
    void dedupe_preserve_order(Seq& v);

}

// Template implementation
#include "container_utils_impl.h"