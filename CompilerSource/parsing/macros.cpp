/**
 * @file  macros.cpp
 * @brief System source implementing functions for parsing macros.
 * 
 * In general, the implementation is unremarkable.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/
/*
#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#include "macros.h"
#include <General/parse_basics.h>
#include <General/debug_macros.h>
#include <System/lex_cpp.h>
using namespace jdi;

string macro_type::NameAndPrototype() const {
  string res = name;
  if (is_function) {
    res += "(";
    for (size_t i = 0; i < params.size(); i++)
      res += params[i]
          + (i + 1 < params.size() ? ", " : is_variadic ? "..." : "");
    res += ")";
  }
  return res;
}

string macro_type::toString() const {
  string res = "#define " + NameAndPrototype() + " \\\n";
  for (size_t i = 0; i < raw_value.size(); ++i)
    res += "  " + raw_value[i].to_string()
        + (i + 1 < raw_value.size()? "\\\n" : "");
  return res;
}

constexpr char kConcatenationError[] =
    "Concatenation marker cannot appear at either end of a replacement list.";

inline bool meaningful_span(const token_vector &tokens, size_t b, size_t e) {
  for (auto i = b; i < e; ++i) {
    if (!tokens[i].preprocesses_away()) return true;
  }
  return false;
}

vector<macro_type::FuncComponent> macro_type::componentize(
    const token_vector &tokens, const vector<string> &params,
    ErrorHandler *herr) {
  vector<macro_type::FuncComponent> res;
  std::unordered_map<std::string_view, size_t> params_by_name;
  for (size_t i = 0; i < params.size(); ++i) params_by_name[params[i]] = i;
  size_t e = 0;
  for (size_t i = 0; i < tokens.size(); ++i) {
    // -------------------------------------------------------------------------
    // -- Handle concatenation markers. ----------------------------------------
    // -------------------------------------------------------------------------
    if (tokens[i].type == TTM_CONCAT) {
      if (!i) {
        herr->error(tokens[i]) << "Concatenation marker (`##`) "
            "cannot appear at either end of a replacement list.";
        continue;
      }
      // Don't push spans CONCAT would always discard.
      if (meaningful_span(tokens, e, i))
        res.push_back(FuncComponent::TokenSpan{e, i});
      res.push_back(FuncComponent::Paste{});
      while (++i < tokens.size() && tokens[i].preprocesses_away());
      e = i--;
      continue;
    }
    // -------------------------------------------------------------------------
    // -- Handle argument names, VA_LIST, VA_OPT. ------------------------------
    // -------------------------------------------------------------------------
    if (tokens[i].type == TT_IDENTIFIER) {
      auto found = params_by_name.find(tokens[i].content.view());
      size_t arg_num = 0;
      if (found == params_by_name.end()) {
        if (tokens[i].content.view() == "__VA_OPT__") {
          if (e != i) res.push_back(FuncComponent::TokenSpan{e, i});
          res.push_back(FuncComponent::VAOpt{});
          e = i + 1;
        } else if (tokens[i].content.view() == "__VA_ARGS__") {
          arg_num = params.size() - 1;
        } else {
          continue;
        }
      } else {
        arg_num = found->second;
      }
      if (e != i) res.push_back(FuncComponent::TokenSpan{e, i});
      // Check if either neighboring token is CONCAT.
      // We already know if the previous was CONCAT, because we'll have
      // a PASTE at the end of our result vector. We must still look ahead.
      size_t j = i;  // First, find next non-whitespace token.
      while (++j < tokens.size() && tokens[j].preprocesses_away());
      // If the next meaningful token is CONCAT, or our last operation is
      // PASTE, we use the raw argument token vector.
      if ((j < tokens.size() && tokens[j].type == TTM_CONCAT) ||
          (!res.empty() && res.back().tag == FuncComponent::PASTE)) {
        res.push_back(FuncComponent::RawArgument{arg_num});
      } else {
        res.push_back(FuncComponent::ExpandedArgument{arg_num});
      }
      e = i + 1;
    } else if (tokens[i].type == TTM_TOSTRING) {
      size_t j = i;
      while (++j < tokens.size() && tokens[j].preprocesses_away());
      if (j >= tokens.size() || tokens[j].type != TT_IDENTIFIER) {
        herr->error(tokens[i], "# must be followed by a parameter name");
        continue;
      }
      if (e != i) res.push_back(FuncComponent::TokenSpan{e, i});
      i = j;
      auto found = params_by_name.find(tokens[i].content.view());
      if (found == params_by_name.end()) {
        herr->error(tokens[i])
            << "# must be followed by a parameter name; "
            << tokens[i].content.toString() << " is not a parameter";
        continue;
      }
      res.push_back(FuncComponent::Stringify{found->second});
      e = i + 1;
    }
  }
  if (e < tokens.size())
    res.push_back(FuncComponent::TokenSpan{e, tokens.size()});
  return res;
}

static token_t paste_tokens(
    const token_t &left, const token_t &right, ErrorHandler *herr) {
  string buf = left.content.toString() + right.content.toString();
  llreader read("token concatenation", buf, false);
  token_t res = read_token(read, herr);
  if (res.type == TT_INVALID || res.type == TT_ENDOFCODE || !read.eof()) {
    herr->error(left) << "Concatenation of " << left << " and " << right
                      << " does not yield a coherent token (got "
                      << res << ")";
  }
  return res;
}

token_vector macro_type::evaluate_concats(const token_vector &replacement_list,
                                          ErrorHandler *herr) {
  token_vector res;
  res.reserve(replacement_list.size());
  for (size_t i = 0; i < replacement_list.size(); ++i) {
    if (replacement_list[i].type == TTM_CONCAT) {
      const size_t cat_at = i;
      while (++i < replacement_list.size() &&
             replacement_list[i].preprocesses_away());
      while (!res.empty() && res.back().preprocesses_away()) res.pop_back();
      if (i >= replacement_list.size() || res.empty()) {
        herr->error(replacement_list[cat_at], kConcatenationError);
        break;
      }
      res.back() = paste_tokens(res.back(), replacement_list[i], herr);
    } else {
      res.push_back(replacement_list[i]);
    }
  }
  return res;
}

static void append_or_paste(token_vector &dest,
                            token_vector::const_iterator begin,
                            token_vector::const_iterator end,
                            bool paste, ErrorHandler *herr) {
  if (begin == end) return;
  if (paste) {
    while (!dest.empty() && dest.back().preprocesses_away()) dest.pop_back();
    while (begin != end && begin->preprocesses_away()) ++begin;
    if (!dest.empty() && begin != end) {
      token_t &left = dest.back();
      left = paste_tokens(left, *begin++, herr);
    }
  }
  dest.insert(dest.end(), begin, end);
}

token_vector macro_type::substitute_and_unroll(
    const vector<token_vector> &args, const vector<token_vector> &args_evald,
    ErrorContext errc) const {
  token_vector res;
  bool paste_next = false;
  if (args.size() < params.size()) {
    if (!is_variadic || args.size() + 1 < params.size()) {
      errc.error() << "Too few arguments to macro " << NameAndPrototype()
                   << ": wanted " << params.size() << ", got " << args.size();
    }
  }
  if (args.size() > params.size()) {
    if (!is_variadic) {
      errc.error() << "Too many arguments to macro " << NameAndPrototype()
                   << ": wanted " << params.size() << ", got " << args.size();
    } else {
      errc.error("Internal error: variadic macro passed too many arguments");
    }
  }
  // Errors from here on out will concern tokens.
  ErrorHandler *herr = errc.get_herr();
  for (const FuncComponent &part : parts) {
    switch (part.tag) {
      case FuncComponent::TOKEN_SPAN:
        append_or_paste(res, raw_value.begin() + part.token_span.begin,
                             raw_value.begin() + part.token_span.end,
                        paste_next, herr);
        paste_next = false;
        break;
      case FuncComponent::RAW_ARGUMENT:
      case FuncComponent::EXPANDED_ARGUMENT:
      case FuncComponent::STRINGIFY: {
        size_t ind = part.raw_expanded_or_stringify_argument.index;
        if (ind >= args.size()) {
          if (ind >= params.size()) {
            errc.error() << "Internal error: "
                << "Macro function built with bad argument references. Index "
                << ind << " out of bounds (only " << params.size()
                << " params defined).";
            paste_next = false;
            continue;
          } else {
            paste_next = false;
            continue;
          }
        }
        if (part.tag == FuncComponent::STRINGIFY) {
          string str;
          for (const token_t &tok : args[ind])
            str += tok.content.toString();
          str = quote(str);
          const string &name_str = params[ind];
          token_vector vec{token_t(TT_STRINGLITERAL, name_str, 0, 0, std::move(str))};
          append_or_paste(res, vec.begin(), vec.end(), paste_next, herr);
          paste_next = false;
          break;
        }
        const token_vector &vec =
            part.tag == FuncComponent::EXPANDED_ARGUMENT
                ? args_evald[ind] : args[ind];
        append_or_paste(res, vec.begin(), vec.end(), paste_next, herr);
        paste_next = false;
        break;
      }
      case FuncComponent::PASTE:
        paste_next = true;
        break;
      case FuncComponent::VA_OPT: {
        token_vector opt;
        if (args.size() == params.size() + 1 && !args[params.size()].empty()) {
          opt.push_back(token_t(TT_COMMA, "__VA_OPT__", 0, 0, ",", 1));
        } else {
          token_vector empty;
          append_or_paste(res, empty.begin(), empty.end(), paste_next, herr);
        }
        break;
      }
      default:
        errc.error("Internal error: Macro function component unknown...");
    }
  }
  return res;
}

*/
