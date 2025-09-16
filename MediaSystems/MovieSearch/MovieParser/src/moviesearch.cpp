#include "moviesearch.h"
#include "utils/string_utils.h"
#include "utils/cmdline_utils.h"
#include "utils/container_utils.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

namespace moviesearch {

    std::vector<std::string> tokenize_command_line(const std::string& raw) {
        std::string line = raw;
        utils::normalize_dashes(line);

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
            auto [opt, attached] = utils::split_opt_equals(t);
            if (!attached.empty() && utils::token_is_option(opt)) {
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
            if (!utils::token_is_option(tok)) {
                res.warnings.push_back("Ignoring unexpected token: '" + tok + "'");
                continue;
            }
            if (utils::matches_option(tok, "title")) {
                auto vals = utils::collect_value_tokens(args, i);
                if (vals.empty()) {
                    res.errors.push_back("Missing value for --title");
                    continue;
                }
                // Treat whitespace-separated words as required keywords; quoted phrases stay intact.
                for (const auto& v : vals) {
                    // If the user wanted to pass comma-separated title tokens, accept those too.
                    auto parts = utils::split_commas_preserve(v);
                    if (parts.empty()) q.title_keywords.push_back(v);
                    else q.title_keywords.insert(q.title_keywords.end(), parts.begin(), parts.end());
                }
            }
            else if (utils::matches_option(tok, "year")) {
                auto vals = utils::collect_value_tokens(args, i);
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
            else if (utils::matches_option(tok, "genre") || utils::matches_option(tok, "genres")) {
                auto vals = utils::collect_value_tokens(args, i);
                if (vals.empty()) {
                    res.errors.push_back("Missing value for --genre");
                    continue;
                }
                for (const auto& v : vals) {
                    auto parts = utils::split_commas_preserve(v);
                    if (parts.empty()) q.genres.push_back(v);
                    else q.genres.insert(q.genres.end(), parts.begin(), parts.end());
                }
            }
            else if (utils::matches_option(tok, "tag") || utils::matches_option(tok, "tags")) {
                auto vals = utils::collect_value_tokens(args, i);
                if (vals.empty()) {
                    res.errors.push_back("Missing value for --tag");
                    continue;
                }
                for (const auto& v : vals) {
                    auto parts = utils::split_commas_preserve(v);
                    if (parts.empty()) q.tags.push_back(v);
                    else q.tags.insert(q.tags.end(), parts.begin(), parts.end());
                }
            }
            else {
                res.errors.push_back("Unknown option: '" + tok + "'");
                // skip any following non-option tokens for this unknown flag
                while (i < args.size() && !utils::token_is_option(args[i])) ++i;
            }
        }

        // Require at least one filter
        if (q.title_keywords.empty() && !q.has_year && q.genres.empty() && q.tags.empty()) {
            res.errors.push_back("moviesearch requires at least one filter (--title/--year/--genre/--tag)");
        }

        // Dedupe lists while preserving order
        utils::dedupe_preserve_order(q.title_keywords);
        utils::dedupe_preserve_order(q.genres);
        utils::dedupe_preserve_order(q.tags);

        res.ok = res.errors.empty();
        return res;
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
