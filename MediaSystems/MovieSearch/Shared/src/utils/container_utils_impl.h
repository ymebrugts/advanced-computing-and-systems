/**
* author Yme Brugts (s4536622)
 * @file container_utils_impl.h
 * @date 2025-01-16
 */

#pragma once
/**
 * @file container_utils_impl.h
 * @brief Template implementations for container utilities
 */

#include <unordered_set>

namespace shared::utils {

    template <class Seq>
    void dedupe_preserve_order(Seq& v) {
        std::unordered_set<std::string> seen;
        Seq out;
        out.reserve(v.size());
        for (auto& x : v) {
            if (seen.insert(x).second) out.push_back(std::move(x));
        }
        v.swap(out);
    }

} // namespace shared::utils