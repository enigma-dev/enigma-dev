SOURCES += $(wildcard Graphics_Systems/OpenGLES/*.cpp) $(wildcard Graphics_Systems/OpenGLES/wrappers/*.cpp) $(wildcard Graphics_Systems/General/*.cpp) ../../android/external/glad/gles2/src/glad.c

override CXXFLAGS += -IBridges/OpenGLES -I../../android/external/glad/gles2/include -IGraphics_Systems/OpenGLES/wrappers
override CFLAGS += -I../../android/external/glad/gles2/include

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lopengl32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL
endif
