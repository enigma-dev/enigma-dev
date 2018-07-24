include Graphics_Systems/OpenGLES/Makefile
override CXXFLAGS += -IGraphics_Systems/OpenGLES3/glad/ -DENIGMA_GRAPHICS_OPENGLES3
SOURCES += $(wildcard Graphics_Systems/OpenGLES3/*.cpp) $(wildcard Graphics_Systems/OpenGL3/*.cpp) Graphics_Systems/OpenGLES3/glad/glad.c
SOURCES := $(filter-out Graphics_Systems/OpenGL3/GL3version.cpp, $(SOURCES))

VERTEX_SHADER_PREFIX := Graphics_Systems/OpenGL3/vertex_shader_prefix.glsl
VERTEX_SHADER := Graphics_Systems/OpenGL3/vertex_shader.glsl
FRAGMENT_SHADER_PREFIX := Graphics_Systems/OpenGL3/fragment_shader_prefix.glsl
FRAGMENT_SHADER := Graphics_Systems/OpenGL3/fragment_shader.glsl
