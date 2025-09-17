/**
* author Yme Brugts (s4536622)
 * @file string_utils.cpp
 * @date 2025-01-16
 */

#include "string_utils.h"

#include <algorithm>
#include <cctype>

namespace shared::utils {

    std::string ltrim(std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            [](unsigned char ch) { return !std::isspace(ch); }));
        return s;
    }

    std::string rtrim(std::string s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
            [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
        return s;
    }

    std::string trim(std::string s) { 
        return rtrim(ltrim(std::move(s))); 
    }

    void replaceAll(std::string& s, const std::string& from, const std::string& to) {
        if (from.empty()) return;
        std::size_t pos = 0;
        while ((pos = s.find(from, pos)) != std::string::npos) {
            s.replace(pos, from.size(), to);
            pos += to.size();
        }
    }

    std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t pos = 0, start = 0;
        while ((pos = s.find(delimiter, start)) != std::string::npos) {
            tokens.push_back(s.substr(start, pos - start));
            start = pos + delimiter.length();
        }
        tokens.push_back(s.substr(start));
        return tokens;
    }

}
