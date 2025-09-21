/**
* author Yme Brugts (s4536622)
 * @file container_utils_impl.h
 * @date 2025-09-16
 */

#pragma once


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

}