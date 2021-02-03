#ifndef ENIGMA_FRONTEND_H
#define ENIGMA_FRONTEND_H
#include <string>
using namespace std;

/// Structure for reporting syntax issues to the IDE.
struct syntax_error {
  const char *err_str; ///< The text associated with the error.
  int line; ///< The line number on which the error occurred.
  int position; ///< The column number in the line at which the error occurred.
  int absolute_index; ///< The zero-based position in the entire code at which the error occurred.

  /// Set the error content in one fell swoop.
  void set(int x, int y,int a, string s);
};

/// Static instance so we don't end up with
extern syntax_error ide_passback_error;

/// An std::string to handle allocation and free for the error string passed to the IDE.
extern string error_sstring;

/// A way to signal the compilation to stop.
extern volatile bool build_stopping;
/// Must be enabled a priori.
extern volatile bool build_enable_stop;

#endif
