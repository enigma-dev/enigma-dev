#define String const char*
#define boolean int

typedef struct _GmObject {
	String name;
	int id;

	Sprite *sprite;
	boolean solid;
	boolean visible;
	int depth;
	boolean persistent;
	struct _GmObject *parent;
	Sprite *mask;

	//MainEvent[] mainEvents;
	int mainEventCount;
} GmObject, *pGmObject;
