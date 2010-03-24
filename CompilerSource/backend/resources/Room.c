#define String const char*
#define boolean int

typedef struct {
	String name;
	int id;

	String caption;
	int width;
	int height;
	int snapX;
	
	int snapY;
	boolean isometric;
	int speed;
	boolean persistent;
	int backgroundColor; //GM color
	boolean drawBackgroundColor;
	String creationCode;

	// vvv  this stuff is probably useless  vvv //
	boolean rememberWindowSize;
	int editorWidth;
	int editorHeight;
	boolean showGrid;
	boolean showObjects;
	boolean showTiles;
	boolean showBackgrounds;
	boolean showViews;
	boolean deleteUnderlyingObjects;
	boolean deleteUnderlyingTiles;
	int currentTab;
	int scrollBarX;
	int scrollBarY;
	boolean enableViews;
	// ^^^  this stuff is probably useless  ^^^ //

	/*BackgroundDef[] backgroundDefs;
	int backgroundDefCount;
	View[] views;
	int viewCount;*/
	Instance[] instances;
	int instanceCount;
	//Tile[] tiles;
	//int tileCount;
} Room, *pRoom;
