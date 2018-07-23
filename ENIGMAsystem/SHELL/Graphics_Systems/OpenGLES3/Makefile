include Graphics_Systems/OpenGL/Makefile
override CXXFLAGS += -IGraphics_Systems/OpenGLES3/glad/ -DENIGMA_GRAPHICS_OPENGLES3
SOURCES += $(wildcard Graphics_Systems/OpenGLES3/*.cpp) $(wildcard Graphics_Systems/OpenGL3/*.cpp) Graphics_Systems/OpenGLES3/glad/glad.c
SOURCES := $(filter-out GL3version.cpp, $(SOURCES))
