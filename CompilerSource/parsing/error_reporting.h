#ifndef ENIGMA_COMPILER_PARSING_ERROR_REPORTING_h
#define ENIGMA_COMPILER_PARSING_ERROR_REPORTING_h

#include <string>
#include <string_view>
#include <sstream>

namespace enigma {
namespace parsing {

class ErrorHandler {
 public:
  virtual void ReportError(size_t line, size_t pos, std::string_view error) = 0;
  virtual void ReportWarning(size_t line, size_t pos, std::string_view warning) = 0;

  class ErrorStream {
   public:
    template<typename T> ErrorStream &operator<<(const T &t) {
      sstream_ << t;
      return *this;
    }

    ErrorStream(ErrorHandler *herr, bool is_warning, size_t line, size_t pos):
        herr_(herr), is_warning_(is_warning), line_(line), pos_(pos) {}

    ~ErrorStream() {
      if (is_warning_) herr_->ReportWarning(line_, pos_, sstream_.str());
      else herr_->ReportError(line_, pos_, sstream_.str());
    }

   private:
    ErrorHandler *const herr_;
    std::stringstream sstream_;
    const bool is_warning_;
    const size_t line_, pos_;
  };

  ErrorStream Error(size_t line, size_t pos) {
    return ErrorStream(this, false, line, pos);
  }

  ErrorStream Warning(size_t line, size_t pos) {
    return ErrorStream(this, true, line, pos);
  }
};

struct ErrorContext {
  ErrorHandler *herr;
  size_t line, pos;
  
  ErrorHandler::ErrorStream Error(size_t line, size_t pos) {
    return herr->Error(line, pos);
  }

  ErrorHandler::ErrorStream Warning(size_t line, size_t pos) {
    return herr->Warning(line, pos);
  }
};

}  // namespace parsing
}  // namespace enigma

#endif
