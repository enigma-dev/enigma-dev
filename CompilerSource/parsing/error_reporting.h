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

#ifndef ENIGMA_COMPILER_PARSING_ERROR_REPORTING_h
#define ENIGMA_COMPILER_PARSING_ERROR_REPORTING_h

#include <string>
#include <string_view>
#include <sstream>
#include <vector>

namespace enigma {
namespace parsing {

struct CodeSnippet {
  /// Token content within the code. Can be used to obtain position/length info.
  std::string_view content;
  size_t line;  ///< Line number at which this token appears.
  size_t position;  ///< Position within that line.
};

struct CodeError : CodeSnippet {
  std::string message;

  std::string ToString() {
    return std::to_string(line) + ":" + std::to_string(position) + ": "
         + message;
  }

  CodeError(CodeSnippet snippet, std::string message_text):
      CodeSnippet(snippet), message(std::move(message_text)) {}
};

struct ErrorContext;

class ErrorHandler {
 public:
  virtual void ReportError(CodeSnippet snippet, std::string_view error) = 0;
  virtual void ReportWarning(CodeSnippet snippet, std::string_view warning) = 0;

  class ErrorStream {
   public:
    template<typename T> ErrorStream &operator<<(const T &t) {
      sstream_ << t;
      return *this;
    }

    ErrorStream(ErrorHandler *herr, bool is_warning, CodeSnippet snippet):
        herr_(herr), is_warning_(is_warning), snippet_(snippet) {}

    ~ErrorStream() {
      if (is_warning_) herr_->ReportWarning(snippet_, sstream_.str());
      else herr_->ReportError(snippet_, sstream_.str());
    }

   private:
    ErrorHandler *const herr_;
    std::stringstream sstream_;
    const bool is_warning_;
    const CodeSnippet snippet_;
  };

  ErrorStream Error(CodeSnippet snippet) {
    return ErrorStream(this, false, snippet);
  }

  ErrorStream Warning(CodeSnippet snippet) {
    return ErrorStream(this, true, snippet);
  }

  inline ErrorContext At(CodeSnippet snippet);
};

struct ErrorContext {
  ErrorHandler *const herr;
  const CodeSnippet snippet;

  ErrorHandler::ErrorStream Error() const {
    return herr->Error(snippet);
  }

  ErrorHandler::ErrorStream Warning() const {
    return herr->Warning(snippet);
  }

  ErrorContext(ErrorHandler *eh, CodeSnippet snip): herr(eh), snippet(snip) {}
};

ErrorContext ErrorHandler::At(CodeSnippet snippet) {
  return ErrorContext(this, snippet);
}

struct ErrorCollector : ErrorHandler {
  // All reported errors.
  std::vector<CodeError> errors;
  std::vector<CodeError> warnings;

  void ReportError(CodeSnippet snippet, std::string_view error) override {
    errors.emplace_back(snippet, std::string{error});
  }
  void ReportWarning(CodeSnippet snippet, std::string_view warning) override {
    warnings.emplace_back(snippet, std::string{warning});
  }
};

}  // namespace parsing
}  // namespace enigma

#endif
