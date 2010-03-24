#define String const char*
#define boolean int

typedef struct {
	int x;
	int y;
	GmObject *object;
	int id;
	String creationCode;
	boolean locked;

	Room *room;
} Instance, *pInstance;
