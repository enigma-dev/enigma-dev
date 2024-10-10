#include <sys/resource.h> //setpriority
#include <unistd.h>
#include <sys/types.h> //getpid

void set_program_priority(int value) {
  setpriority(PRIO_PROCESS, getpid(), value);
}
