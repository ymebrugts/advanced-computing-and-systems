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
    	std::vector<std::string> tokens;
        std::string cur;

        for (char c : raw) {
            if (std::isspace(static_cast<unsigned char>(c))) {
                if (!cur.empty()) {
                    tokens.emplace_back(std::move(cur));
                    cur.clear();
                }
            }
            else {
                cur.push_back(c);
            }
        }

        if (!cur.empty()) {
            tokens.emplace_back(std::move(cur));
        }

        return tokens;
    }

    static void process_moviesearch_param(std::vector<std::string>& dest,
        const std::string& optName,
        const std::vector<std::string>& args,
        std::size_t& i,
        ParseResult& res) {
        auto vals = utils::collect_value_tokens(args, i);
        if (vals.empty()) {
            res.errors.push_back("Missing value for --" + optName);
            return;
        }
        dest.insert(dest.end(), vals.begin(), vals.end());
    }


    ParseResult parse_moviesearch_tokens(const std::vector<std::string>& rawArgs) {
        ParseResult res;
        Query& q = res.query;

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
                process_moviesearch_param(q.title_keywords, "title", args, i, res);
            }
            else if (utils::matches_option(tok, "year")) {
                auto vals = utils::collect_value_tokens(args, i);
                if (vals.empty()) {
                    res.errors.push_back("Missing value for --year");
                    continue;
                }
                if (vals.size() > 1) {
                    res.errors.push_back("Too many values for --year (expected one)");
                    continue;
                }
                try {
                    q.year = std::stoi(vals.front());
                    q.has_year = true;
                }
                catch (...) {
                    res.errors.push_back("Invalid year: '" + vals.front() + "'");
                }
            }
            else if (utils::matches_option(tok, "genre") || utils::matches_option(tok, "genres")) {
                process_moviesearch_param(q.genres, "genre", args, i, res);
            }
            else if (utils::matches_option(tok, "tag") || utils::matches_option(tok, "tags")) {
                process_moviesearch_param(q.tags, "tag", args, i, res);
            }
            else {
                res.errors.push_back("Unknown option: '" + tok + "'");
                while (i < args.size() && !utils::token_is_option(args[i])) ++i; // skip
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
