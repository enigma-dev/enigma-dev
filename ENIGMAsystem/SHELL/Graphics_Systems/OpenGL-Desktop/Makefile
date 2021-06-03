override CXXFLAGS += -IBridges/OpenGL

ifeq ($(TARGET_PLATFORM), Windows)
	override CXXFLAGS += -DGLEW_STATIC
	override LDLIBS += -lglew32 -lopengl32
else ifeq ($(TARGET_PLATFORM), Linux)
	override LDLIBS += -lGL -lGLEW
else ifeq ($(TARGET_PLATFORM), FreeBSD)
	override LDLIBS += -lGL -lGLEW
else ifeq ($(TARGET_PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL -lGLEW
endif
