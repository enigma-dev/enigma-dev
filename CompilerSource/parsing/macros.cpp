/** Copyright (C) 2020 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "macros.h"

#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#include "macros.h"
#include "lexer.h"
using namespace jdi;

using std::string;

namespace enigma {
namespace parsing {
namespace {

string QuoteString(std::string_view str) {
  std::string res;
  res.reserve(str.length() * 2 + 2);
  res += '"';
  for (char c : str) switch (c) {
    case '\r': res += "\\r"; break;
    case '\n': res += "\\n"; break;
    case '\\': res += "\\\\"; break;
    case '\"': res += "\\\""; break;
    case '\'': res += "\\\'"; break;
    default: res += c;
  }
  res += '"';
  return res;
}

}  // namespace

string Macro::NameAndPrototype() const {
  string res = name;
  if (parameters) {
    res += "(";
    for (size_t i = 0; i < parameters->size(); i++)
      res += (*parameters)[i]
          + (i + 1 < parameters->size() ? ", " : is_variadic ? "..." : "");
    res += ")";
  }
  return res;
}

string Macro::ToString() const {
  string res = "#define " + NameAndPrototype() + " \\\n";
  for (size_t i = 0; i < value.size(); ++i)
    res += "  " + value[i].ToString() + (i + 1 < value.size()? "\\\n" : "");
  return res;
}

constexpr char kConcatenationError[] =
    "Concatenation marker cannot appear at either end of a replacement list.";

inline bool meaningful_span(const TokenVector &tokens, size_t b, size_t e) {
  for (auto i = b; i < e; ++i) {
    if (!tokens[i].PreprocessesAway()) return true;
  }
  return false;
}

vector<Macro::FuncComponent> Macro::Componentize(
    const TokenVector &tokens, const vector<string> &params,
    ErrorHandler *herr) {
  vector<Macro::FuncComponent> res;
  std::unordered_map<std::string_view, size_t> params_by_name;
  for (size_t i = 0; i < params.size(); ++i) params_by_name[params[i]] = i;
  size_t e = 0;
  for (size_t i = 0; i < tokens.size(); ++i) {
    // -------------------------------------------------------------------------
    // -- Handle concatenation markers. ----------------------------------------
    // -------------------------------------------------------------------------
    if (tokens[i].type == TTM_CONCAT) {
      if (!i) {
        herr->Error(tokens[i]) << "Concatenation marker (`##`) "
            "cannot appear at either end of a replacement list.";
        continue;
      }
      // Don't push spans CONCAT would always discard.
      if (meaningful_span(tokens, e, i))
        res.push_back(FuncComponent::TokenSpan{e, i});
      res.push_back(FuncComponent::Paste{});
      while (++i < tokens.size() && tokens[i].PreprocessesAway()) /* pass */;
      e = i--;
      continue;
    }
    // -------------------------------------------------------------------------
    // -- Handle argument names, VA_LIST, VA_OPT. ------------------------------
    // -------------------------------------------------------------------------
    if (tokens[i].type == TT_VARNAME) {
      auto found = params_by_name.find(tokens[i].content);
      size_t arg_num = 0;
      if (found == params_by_name.end()) {
        if (tokens[i].content == "__VA_OPT__") {
          if (e != i) res.push_back(FuncComponent::TokenSpan{e, i});
          res.push_back(FuncComponent::VAOpt{});
          e = i + 1;
        } else if (tokens[i].content == "__VA_ARGS__") {
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
      while (++j < tokens.size() && tokens[j].PreprocessesAway());
      // If the next meaningful token is CONCAT, or our last operation is
      // PASTE, we use the raw argument token vector.
      if ((j < tokens.size() && tokens[j].type == TTM_CONCAT) ||
          (!res.empty() && res.back().tag == FuncComponent::PASTE)) {
        res.push_back(FuncComponent::RawArgument{arg_num});
      } else {
        res.push_back(FuncComponent::ExpandedArgument{arg_num});
      }
      e = i + 1;
    } else if (tokens[i].type == TTM_STRINGIFY) {
      size_t j = i;
      while (++j < tokens.size() && tokens[j].PreprocessesAway());
      if (j >= tokens.size() || tokens[j].type != TT_VARNAME) {
        herr->Error(tokens[i]) << "# must be followed by a parameter name";
        continue;
      }
      if (e != i) res.push_back(FuncComponent::TokenSpan{e, i});
      i = j;
      auto found = params_by_name.find(tokens[i].content);
      if (found == params_by_name.end()) {
        herr->Error(tokens[i])
            << "# must be followed by a parameter name; "
            << tokens[i].content << " is not a parameter";
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

static string TokContentCat(const Token &left, const Token &right) {
  string content;
  content.reserve(left.content.size() + right.content.size());
  content += left.content;
  content += right.content;
  return content;
}

static Token PasteTokens(
    const Token &left, const Token &right, const ErrorContext &errc) {
  static const MacroMap no_macros;
  static const ParseContext empty_context(ParseContext::EmptyLanguage{});
  Lexer l(std::move(TokContentCat(left, right)), &empty_context, errc.herr);
  Token res = l.ReadRawToken();
  Token end = l.ReadRawToken();
  if (end.type != TT_ENDOFCODE) {
    errc.Error() << "Concatenation of `" << left.content
                 << "` and `" << right.content
                 << "` does not produce a valid token.";
  }
  return res;
}

static void AppendOrPaste(TokenVector &dest,
                          TokenVector::const_iterator begin,
                          TokenVector::const_iterator end,
                          bool paste, const ErrorContext &errc) {
  if (begin == end) return;
  if (paste) {
    while (!dest.empty() && dest.back().PreprocessesAway()) dest.pop_back();
    while (begin != end && begin->PreprocessesAway()) ++begin;
    if (!dest.empty() && begin != end) {
      Token &left = dest.back();
      left = PasteTokens(left, *begin++, errc);
    }
  }
  dest.insert(dest.end(), begin, end);
}

TokenVector Macro::SubstituteAndUnroll(
    const vector<TokenVector> &args, const vector<TokenVector> &args_evald,
    ErrorContext errc) const {
  TokenVector res;
  bool paste_next = false;
  if (args.size() != parameters->size()) {
    if (!is_variadic || args.size() + 1 < parameters->size()) {
      errc.Error()
          << (args.size() > parameters->size()
                  ? "Too many arguments to macro "
                  : "Too few arguments to macro ")
          << NameAndPrototype() << ": wanted " << parameters->size()
          << ", got " << args.size();
    } else if (args.size() > parameters->size()) {
      errc.Error() << "Internal error: variadic macro passed too many arguments";
    }
  }
  // Errors from here on out will concern tokens.
  for (const FuncComponent &part : parts) {
    switch (part.tag) {
      // Copy verbatim from the source macro.
      case FuncComponent::TOKEN_SPAN: {
        AppendOrPaste(res, value.begin() + part.token_span.begin,
                           value.begin() + part.token_span.end,
                      paste_next, errc);
        paste_next = false;
        break;
      }
      // Various kinds of argument substitutions for macro parameters.
      // Arguments can be inserted verbatim, inserted after preprocessing
      // (the default), or converted to a string via #parameter.
      case FuncComponent::RAW_ARGUMENT:
      case FuncComponent::EXPANDED_ARGUMENT:
      case FuncComponent::STRINGIFY: {
        size_t ind = part.raw_expanded_or_stringify_argument.index;
        if (ind >= args.size()) {
          if (ind >= parameters->size()) {
            errc.Error() << "Internal error: "
                << "Macro function built with bad argument references. Index "
                << ind << " out of bounds (only " << parameters->size()
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
          for (const Token &tok : args[ind])
            str += tok.content;
          str = QuoteString(str);
          TokenVector vec{Token(TT_STRING, errc.snippet)};
          AppendOrPaste(res, vec.begin(), vec.end(), paste_next, errc);
          paste_next = false;
          break;
        }
        const TokenVector &vec =
            part.tag == FuncComponent::EXPANDED_ARGUMENT
                ? args_evald[ind] : args[ind];
        AppendOrPaste(res, vec.begin(), vec.end(), paste_next, errc);
        paste_next = false;
        break;
      }
      // The ## operator pasting two tokens together.
      case FuncComponent::PASTE:
        paste_next = true;
        break;
      // The C++ __VA_OPT__ macro, which is a comma if and only if the variadic
      // argument list is not empty. When that list is empty, this token
      // preprocesses away.
      case FuncComponent::VA_OPT: {
        TokenVector opt;
        if (args.size() == parameters->size() + 1 &&
            !args[parameters->size()].empty()) {
          opt.push_back(Token(TT_COMMA, errc.snippet));
        } else {
          TokenVector empty;
          AppendOrPaste(res, empty.begin(), empty.end(), paste_next, errc);
        }
        break;
      }
      default:
        errc.Error() << "Internal error: Macro function component unknown...";
    }
  }
  return res;
}

}  // namespace parsing
}  // namespace enigma
