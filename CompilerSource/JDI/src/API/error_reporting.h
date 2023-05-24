/**
 * @file  error_reporting.h
 * @brief Header declaring a base class for error and warning reporting.
 *
 * Also defines a default error handling class, which shall write all warnings
 * and errors to stderr.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef JDI_ERROR_REPORTING_h
#define JDI_ERROR_REPORTING_h

#include <string>
#include <string_view>
#include <sstream>
#include "General/strings.h"

namespace jdi {

struct SourceLocation;
class ErrorHandler;
class ErrorContext;
class ErrorStream;

enum ErrorLevel { ERROR, WARNING, INFO };

struct SourceLocation {
  static constexpr size_t npos = (size_t) -1;
  std::string filename;
  size_t line;
  size_t pos;

  template<typename T,
           typename e = decltype(std::declval<T>().get_filename()),
           typename e2 = decltype(std::declval<T>().get_line_number()),
           typename e3 = decltype(std::declval<T>().get_line_position())>
  SourceLocation(const T &code_point):
      filename(code_point.get_filename()),
      line(code_point.get_line_number()),
      pos(code_point.get_line_position()) {}
  SourceLocation(std::string_view file, size_t l, size_t p):
      filename(file), line(l), pos(p) {}

  std::string to_string() const;
};

/// Abstract class for error handling and warning reporting.
/// Implement this class yourself, or use \c default_error_handler.
class ErrorHandler {

 public:
  /// Method invoked when an error occurs. In addition to the error,
  /// information about the source of the error is included. The parser
  /// attempts to choose the most relevant source location when supplying
  /// this information.
  virtual void error(std::string_view err, SourceLocation code_point) = 0;
  /// Used like `error()`, but called for warnings.
  virtual void warning(std::string_view err, SourceLocation code_point) = 0;
  /// Used like `warning()`, but called with general information, likely
  /// explaining a previous error.
  virtual void info(std::string_view err, int verbosity,
                    SourceLocation code_point) = 0;

  // Formatting helpers.
  template<typename... Args>
  void error(const SourceLocation &sl, std::string_view msg, Args... args) {
    return error(format(msg, args...), sl);
  }
  template<typename... Args>
  void warning(const SourceLocation &sl, std::string_view msg, Args... args) {
    return warning(format(msg, args...), sl);
  }

  /// Bind to a SourceLocation, creating an ErrorContext.
  inline ErrorContext at(SourceLocation sloc);

  // Streams.
  inline ErrorStream error(SourceLocation _sloc);
  inline ErrorStream warning(SourceLocation _sloc);
  inline ErrorStream info(SourceLocation _sloc);

  /// Virtual destructor in case children have additional data types to free.
  virtual ~ErrorHandler() = default;
};

class ErrorContext {
  ErrorHandler *herr_;
  SourceLocation sloc_;

 public:
  void error(std::string_view err) const { herr_->error(err, sloc_); }
  void warning(std::string err) const { herr_->warning(err, sloc_); }
  void info(int v, std::string err) const { herr_->info(err, v, sloc_); }
  ErrorHandler *get_herr()  const { return herr_; }
  SourceLocation get_source_location() const { return sloc_; }

  // Streams.
  inline ErrorStream error() const;
  inline ErrorStream warning() const;
  inline ErrorStream info() const;

  /// Construct from an ErrorHandler and source location.
  ErrorContext(ErrorHandler *h, SourceLocation sloc): herr_(h), sloc_(sloc) {}
};

class ErrorStream {
  ErrorHandler *herr_;
  SourceLocation sloc_;
  std::stringstream message_;
  ErrorLevel level_;

 public:
  explicit ErrorStream(ErrorHandler *herr, const SourceLocation &loc,
                       ErrorLevel level):
      herr_(herr), sloc_(loc), level_(level) {}
  template<typename T>
  ErrorStream &operator<<(const T &x) {
    string_detection::put(message_, x);
    return *this;
  }
  ~ErrorStream() {
    switch (level_) {
      default:
      case ERROR:   herr_->  error(message_.str(), sloc_); return;
      case WARNING: herr_->warning(message_.str(), sloc_); return;
      case INFO:    herr_->   info(message_.str(), 0, sloc_); return;
    }
  }
};

ErrorContext ErrorHandler::at(SourceLocation sloc) {
  return ErrorContext(this, sloc);
}

ErrorStream ErrorHandler::error(SourceLocation sloc) {
  return ErrorStream(this, sloc, ErrorLevel::ERROR);
}
ErrorStream ErrorHandler::warning(SourceLocation sloc) {
  return ErrorStream(this, sloc, ErrorLevel::WARNING);
}
ErrorStream ErrorHandler::info(SourceLocation sloc) {
  return ErrorStream(this, sloc, ErrorLevel::INFO);
}

ErrorStream ErrorContext::error() const {
  return ErrorStream(herr_, sloc_, ErrorLevel::ERROR);
}
ErrorStream ErrorContext::warning() const {
  return ErrorStream(herr_, sloc_, ErrorLevel::WARNING);
}
ErrorStream ErrorContext::info() const {
  return ErrorStream(herr_, sloc_, ErrorLevel::INFO);
}

/// Default class for error handling and warning reporting.
/// Prints all errors and warnings to stderr alike. No other action is taken.
struct DefaultErrorHandler: ErrorHandler {
  unsigned error_count, warning_count;
  /// Prints the error to stderr, in the format
  /// `ERROR[(<file>[:<line>[:<pos>]])]: <error string>`
  virtual void error(std::string_view msg, SourceLocation code_point);
  /// Prints the warning to stderr, in the format
  /// `Warning[(<file>[:<line>[:<pos>]])]: <warning string>`
  virtual void warning(std::string_view msg, SourceLocation code_point);
  /// Prints the warning to stderr, in the format
  /// `Note[(<file>[:<line>[:<pos>]])]: <info string>`
  virtual void info(std::string_view msg, int verbosity,
                    SourceLocation code_point);
  DefaultErrorHandler();
};

/// A pointer to an instance of \c DefaultErrorHandler, for use wherever.
extern DefaultErrorHandler *default_error_handler;

}  // namespace
#endif  // JDI_ERROR_REPORTING_h
