#ifndef JDI_TESTING_ERROR_HANDLER_h
#define JDI_TESTING_ERROR_HANDLER_h

#include <API/error_reporting.h>

namespace {

class ErrorConstitutesFailure : public jdi::ErrorHandler {
  void error(std::string_view err, jdi::SourceLocation sloc) final {
    ADD_FAILURE() << "Underlying code reported an error: " << err
                  << " (at " << sloc.filename << ":" << sloc.line << ":"
                  << sloc.pos << ")";
  }
  void warning(std::string_view err, jdi::SourceLocation sloc) final {
    ADD_FAILURE() << "Underlying code reported a warning: " << err
                  << " (at " << sloc.filename << ":" << sloc.line << ":"
                  << sloc.pos << ")";
  }
  void info(std::string_view, int, jdi::SourceLocation) final {}
} *error_constitutes_failure = new ErrorConstitutesFailure;

}  // namespace

#endif  // JDI_TESTING_ERROR_HANDLER_h
