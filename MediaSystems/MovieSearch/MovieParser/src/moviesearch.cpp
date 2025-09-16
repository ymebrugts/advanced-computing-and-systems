#include "moviesearch.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

namespace moviesearch {

    namespace {

        // ---------- small string helpers ----------
        static inline std::string ltrim(std::string s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                [](unsigned char ch) { return !std::isspace(ch); }));
            return s;
        }
        static inline std::string rtrim(std::string s) {
            s.erase(std::find_if(s.rbegin(), s.rend(),
                [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
            return s;
        }
        static inline std::string trim(std::string s) { return rtrim(ltrim(std::move(s))); }

        static inline void replaceAll(std::string& s, const std::string& from, const std::string& to) {
            if (from.empty()) return;
            std::size_t pos = 0;
            while ((pos = s.find(from, pos)) != std::string::npos) {
                s.replace(pos, from.size(), to);
                pos += to.size();
            }
        }

        // UTF‑8 normalize en/em dash to ASCII hyphen-minus
        static inline void normalize_dashes(std::string& s) {
            replaceAll(s, "\xE2\x80\x93", "-"); // – en
            replaceAll(s, "\xE2\x80\x94", "-"); // — em
            // best-effort if compiler/source lets these appear directly
            replaceAll(s, "–", "-");
            replaceAll(s, "—", "-");
        }

        static inline bool token_is_option(const std::string& t) {
            return t.size() >= 2 && t[0] == '-';
        }
        static inline bool matches_option(const std::string& tok, const std::string& name) {
            if (tok.size() < 2 || tok[0] != '-') return false;
            std::size_t i = (tok.size() >= 3 && tok[1] == '-') ? 2 : 1;
            return tok.substr(i) == name;
        }

        static inline std::pair<std::string, std::string> split_opt_equals(const std::string& tok) {
            auto p = tok.find('=');
            if (p == std::string::npos) return { tok, "" };
            return { tok.substr(0, p), tok.substr(p + 1) };
        }

        // Return contiguous non-option tokens as separate items (preserves words)
        static std::vector<std::string> collect_value_tokens(const std::vector<std::string>& tokens, std::size_t& i) {
            std::vector<std::string> vals;
            while (i < tokens.size() && !token_is_option(tokens[i])) {
                if (!tokens[i].empty()) vals.push_back(tokens[i]);
                ++i;
            }
            return vals;
        }

        static std::vector<std::string> split_commas_preserve(const std::string& s) {
            std::vector<std::string> out;
            std::string item;
            std::istringstream iss(s);
            while (std::getline(iss, item, ',')) {
                item = trim(item);
                if (!item.empty()) out.push_back(item);
            }
            return out;
        }

        template <class Seq>
        static void dedupe_preserve_order(Seq& v) {
            std::unordered_set<std::string> seen;
            Seq out;
            out.reserve(v.size());
            for (auto& x : v) {
                if (seen.insert(x).second) out.push_back(std::move(x));
            }
            v.swap(out);
        }

    } // namespace

    std::vector<std::string> tokenize_command_line(const std::string& raw) {
        std::string line = raw;
        normalize_dashes(line);

        std::vector<std::string> tokens;
        std::string cur;
        bool inQuote = false;
        char quoteChar = '\0';

        for (std::size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (inQuote) {
                if (c == '\\') {
                    if (i + 1 < line.size() && line[i + 1] == quoteChar) {
                        cur.push_back(line[i + 1]);
                        ++i;
                    }
                    else {
                        cur.push_back(c);
                    }
                }
                else if (c == quoteChar) {
                    inQuote = false;
                }
                else {
                    cur.push_back(c);
                }
            }
            else {
                if (c == '"' || c == '\'') {
                    inQuote = true;
                    quoteChar = c;
                }
                else if (std::isspace(static_cast<unsigned char>(c))) {
                    if (!cur.empty()) {
                        tokens.emplace_back(std::move(cur));
                        cur.clear();
                    }
                }
                else {
                    cur.push_back(c);
                }
            }
        }
        if (!cur.empty()) tokens.emplace_back(std::move(cur));
        return tokens;
    }

    ParseResult parse_moviesearch_tokens(const std::vector<std::string>& rawArgs) {
        ParseResult res;
        Query& q = res.query;

        // Expand --opt=value into two tokens
        std::vector<std::string> args;
        args.reserve(rawArgs.size());
        for (const auto& t : rawArgs) {
            auto [opt, attached] = split_opt_equals(t);
            if (!attached.empty() && token_is_option(opt)) {
                args.push_back(opt);
                args.push_back(attached);
            }
            else {
                args.push_back(t);
            }
        }

        std::size_t i = 0;
        while (i < args.size()) {
            const std::string tok = args[i++];
            if (!token_is_option(tok)) {
                res.warnings.push_back("Ignoring unexpected token: '" + tok + "'");
                continue;
            }
            if (matches_option(tok, "title")) {
                auto vals = collect_value_tokens(args, i);
                if (vals.empty()) {
                    res.errors.push_back("Missing value for --title");
                    continue;
                }
                // Treat whitespace-separated words as required keywords; quoted phrases stay intact.
                for (const auto& v : vals) {
                    // If the user wanted to pass comma-separated title tokens, accept those too.
                    auto parts = split_commas_preserve(v);
                    if (parts.empty()) q.title_keywords.push_back(v);
                    else q.title_keywords.insert(q.title_keywords.end(), parts.begin(), parts.end());
                }
            }
            else if (matches_option(tok, "year")) {
                auto vals = collect_value_tokens(args, i);
                if (vals.empty()) {
                    res.errors.push_back("Missing value for --year");
                    continue;
                }
                if (vals.size() > 1) {
                    res.errors.push_back("Too many values for --year (expected a single year)");
                    continue;
                }
                try {
                    int y = std::stoi(vals.front());
                    q.has_year = true;
                    q.year = y;
                }
                catch (...) {
                    res.errors.push_back("Invalid year: '" + vals.front() + "'");
                }
            }
            else if (matches_option(tok, "genre") || matches_option(tok, "genres")) {
                auto vals = collect_value_tokens(args, i);
                if (vals.empty()) {
                    res.errors.push_back("Missing value for --genre");
                    continue;
                }
                for (const auto& v : vals) {
                    auto parts = split_commas_preserve(v);
                    if (parts.empty()) q.genres.push_back(v);
                    else q.genres.insert(q.genres.end(), parts.begin(), parts.end());
                }
            }
            else if (matches_option(tok, "tag") || matches_option(tok, "tags")) {
                auto vals = collect_value_tokens(args, i);
                if (vals.empty()) {
                    res.errors.push_back("Missing value for --tag");
                    continue;
                }
                for (const auto& v : vals) {
                    auto parts = split_commas_preserve(v);
                    if (parts.empty()) q.tags.push_back(v);
                    else q.tags.insert(q.tags.end(), parts.begin(), parts.end());
                }
            }
            else {
                res.errors.push_back("Unknown option: '" + tok + "'");
                // skip any following non-option tokens for this unknown flag
                while (i < args.size() && !token_is_option(args[i])) ++i;
            }
        }

        // Require at least one filter
        if (q.title_keywords.empty() && !q.has_year && q.genres.empty() && q.tags.empty()) {
            res.errors.push_back("moviesearch requires at least one filter (--title/--year/--genre/--tag)");
        }

        // Dedupe lists while preserving order
        dedupe_preserve_order(q.title_keywords);
        dedupe_preserve_order(q.genres);
        dedupe_preserve_order(q.tags);

        res.ok = res.errors.empty();
        return res;
    }

    ParseResult parse_moviesearch_line(const std::string& line) {
        auto tokens = tokenize_command_line(line);
        if (tokens.empty()) return {};
        // Accept lines with or without the leading command name
        if (tokens.front() == "moviesearch") {
            return parse_moviesearch_tokens({ tokens.begin() + 1, tokens.end() });
        }
        else {
            // If the first token isn't 'moviesearch', emit an error but try to parse as args.
            ParseResult r = parse_moviesearch_tokens(tokens);
            if (r.errors.empty()) {
                r.warnings.push_back("Line did not start with 'moviesearch' — parsed as arguments only");
            }
            return r;
        }
    }

    void print_query(std::ostream& out, const Query& q) {
        out << "Parsed movie search (AND semantics):\n";
        out << "  title_keywords : ";
        if (q.title_keywords.empty()) out << "(none)\n";
        else {
            out << "[";
            for (std::size_t i = 0; i < q.title_keywords.size(); ++i) {
                if (i) out << ", ";
                out << "\"" << q.title_keywords[i] << "\"";
            }
            out << "]\n";
        }
        out << "  year           : " << (q.has_year ? std::to_string(q.year) : "(none)") << "\n";
        out << "  genres         : ";
        if (q.genres.empty()) out << "(none)\n";
        else {
            out << "[";
            for (std::size_t i = 0; i < q.genres.size(); ++i) {
                if (i) out << ", ";
                out << q.genres[i];
            }
            out << "]\n";
        }
        out << "  tags           : ";
        if (q.tags.empty()) out << "(none)\n";
        else {
            out << "[";
            for (std::size_t i = 0; i < q.tags.size(); ++i) {
                if (i) out << ", ";
                out << q.tags[i];
            }
            out << "]\n";
        }
        out << "(execution not implemented; parsing only)\n";
    }

} // namespace moviesearch
