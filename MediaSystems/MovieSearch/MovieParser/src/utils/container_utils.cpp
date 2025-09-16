/**
* author Yme Brugts (s4536622)
 * @file container_utils.cpp
 * @date 2025-01-16
 */

#include "container_utils.h"
#include "string_utils.h"

#include <sstream>

namespace moviesearch::utils {

    std::vector<std::string> split_commas_preserve(const std::string& s) {
        std::vector<std::string> out;
        std::string item;
        std::istringstream iss(s);
        while (std::getline(iss, item, ',')) {
            item = trim(item);
            if (!item.empty()) out.push_back(item);
        }
        return out;
    }

} // namespace moviesearch::utils