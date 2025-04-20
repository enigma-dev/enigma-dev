#include "ExtensionExample.h"

/* place helper functions you do not want to be used in the LateralGM IDE
outside the enigma_user namespace, to avoid the functions being added: */

namespace {

void DummyHelperFunction_Placeholder() {
  // TODO: function body goes here.
}

} // anonymous namespace

/* place functions you want to use in the LateralGM IDE in the enigma_user
namespace the functions will then be detected automatically on IDE run: */

namespace enigma_user {

long double ExtensionExample_DoAddNumbers(long double x, long double y) {
  return x + y;
}

std::string ExtensionExample_ConcatString(std::string x, std::string y) {
  return x + y;
}

} // namespace enigma_user
