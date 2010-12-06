#include "../object.h"

#define recast(inst,struct) (*(struct*)((char*)inst+struct##Offset))
//Note: We could alternatively provide alarmStruct with a fluff[0] for explicit memory mapping, and define recast thusly:
//#define recast(inst,struct) (*(struct*)&((struct*)inst)->fluff[struct##Offset])

extern ptrdiff_t alarmStructOffset;
struct alarmStruct { var alarm; };

