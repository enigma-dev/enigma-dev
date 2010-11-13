#include "../object.h"

extern ptrdiff_t alarmStructOffset;
struct alarmStruct { char fluff[alarmStructOffset]; var alarm; }

