#include "Platforms/General/PFmain.h"

#include <unistd.h>
#include <libgen.h>
#include <cstdlib>
#include <climits>
#include <string>

using std::string;

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
