/**
* author Yme Brugts (s4536622)
 * @file string_utils.cpp
 * @date 2025-09-16
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

    // Case-insensitive substring search
    bool insensitive_contains(const std::string& text, const std::string& word) {
        auto it = std::search(
            text.begin(), text.end(),
            word.begin(), word.end(),
            [](unsigned char ch1, unsigned char ch2) {
                return std::tolower(ch1) == std::tolower(ch2);
            }
        );
        return it != text.end();
    }

    bool insensitive_contains_word(const std::string& text, const std::string& word) {
        if (word.empty()) return false;

        // Split text into tokens by spaces (or smarter delimiters later)
        auto tokens = shared::utils::split(text, " ");
        std::string lowerWord;
        lowerWord.reserve(word.size());

        for (char c : word) lowerWord.push_back(std::tolower(static_cast<unsigned char>(c)));

        for (auto& token : tokens) {
            std::string lowerToken;
            lowerToken.reserve(token.size());
            for (char c : token) lowerToken.push_back(std::tolower(static_cast<unsigned char>(c)));

            if (lowerToken == lowerWord) {
                return true;
            }
        }
        return false;
    }

}
