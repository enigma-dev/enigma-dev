#define String const char*

typedef struct {
	int fileVersion;
	String filename;

	Sprite[] sprites;
	/*Sound[] sounds;
	Background[] backgrounds;
	Path[] paths;
	Script[] scripts;
	Font[] fonts;
	Timeline[] timelines;
	GmObject[] gmObjects;
	Room[] rooms;

	Trigger[] triggers;
	Constant[] constants;
	Include[] includes;*/
	String[] packages;

	//GameInformation gameInfo;
	//GameSettings gameSettings;
	int lastInstanceId = 100000;
	int lastTileId = 10000000;
} EnigmaStruct, *pEnigmaStruct;
