override CXXFLAGS += -IGraphics_Systems/OpenGLES2/glad/ -DglClearDepth=glClearDepthf -DGL_BGRA=GL_BGRA_EXT -DENIGMA_GRAPHICS_OPENGLES2
SOURCES += $(wildcard Graphics_Systems/OpenGL3/*.cpp) $(wildcard Graphics_Systems/General/*.cpp) Graphics_Systems/OpenGLES2/glad/glad.c

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lopengl32 -lglu32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL -lGLU -ldl
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL
endif
