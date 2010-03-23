#define String const char*
#define boolean int

typedef struct {
	String name;
	int id;

	boolean transparent;
	int shape; //0*=Precise, 1=Rectangle,  2=Disk, 3=Diamond
	int alphaTolerance;
	boolean separateMask;
	boolean smoothEdges;
	boolean preload;
	int originX;
	int originY;
	int bbMode; //0*=Automatic, 1=Full image, 2=Manual
	int bbLeft;
	int bbRight;
	int bbTop;
	int bbBottom;

	Image[] subImages;
} Sprite, *pSprite;
