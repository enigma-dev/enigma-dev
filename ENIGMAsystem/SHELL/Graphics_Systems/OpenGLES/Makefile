override CXXFLAGS += -IGraphics_Systems/OpenGLES/
SOURCES += $(wildcard Graphics_Systems/OpenGLES/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lopengl32 -lglu32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL -lGLU -ldl
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL
endif
