#define String const char*

typedef struct {
	int fileVersion;
	String filename;

	Sprite[] sprites;
	int spriteCount;
	/*Sound[] sounds;
	int soundCount;
	Background[] backgrounds;
	int backgroundCount;
	Path[] paths;
	int pathCount;
	Script[] scripts;
	int scriptCount;
	Font[] fonts;
	int fontCount;
	Timeline[] timelines;
	int timelineCount;
	GmObject[] gmObjects;
	int gmObjectCount;
	Room[] rooms;
	int roomCount;

	Trigger[] triggers;
	int triggerCount;
	Constant[] constants;
	int constantCount;
	Include[] includes;
	int includeCount;*/
	String[] packages;
	int packageCount;

	//GameInformation gameInfo;
	//GameSettings gameSettings;
	int lastInstanceId;
	int lastTileId;
} EnigmaStruct, *pEnigmaStruct;
