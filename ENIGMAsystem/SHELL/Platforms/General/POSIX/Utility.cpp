#include "Utility.h"

#include <unistd.h> //usleep

namespace enigma {
void Sleep(int ms)
{
  if(ms>=1000) enigma_user::sleep(ms/1000);
  if(ms>0) usleep(ms%1000*1000);
}

} //namespace enigma

namespace enigma_user {
void sleep(int ms) { enigma::Sleep(ms); }
} //namespace enigma_user
