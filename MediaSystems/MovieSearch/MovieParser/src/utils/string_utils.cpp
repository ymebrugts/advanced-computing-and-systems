#include "string_utils.h"

#include <algorithm>
#include <cctype>

namespace moviesearch {
namespace utils {

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

    void normalize_dashes(std::string& s) {
        replaceAll(s, "\xE2\x80\x93", "-"); // – en
        replaceAll(s, "\xE2\x80\x94", "-"); // — em
        // best-effort if compiler/source lets these appear directly
        replaceAll(s, "–", "-");
        replaceAll(s, "—", "-");
    }

} // namespace utils
} // namespace moviesearch