include Graphics_Systems/OpenGL-OpenGLES/Makefile
override CXXFLAGS += -IGraphics_Systems/OpenGL/ -IBridges/Glew-OpenGL
SOURCES += $(wildcard Graphics_Systems/OpenGL/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)

ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += -DGLEW_STATIC
	override LDLIBS += -lglew32 -lopengl32 -lglu32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL -lGLU -lGLEW
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL -lGLEW
endif
