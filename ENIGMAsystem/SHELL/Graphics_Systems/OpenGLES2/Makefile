include Graphics_Systems/OpenGLES/Makefile
override CXXFLAGS += -IGraphics_Systems/OpenGLES2/glad/ -DENIGMA_GRAPHICS_OPENGLES2
SOURCES += $(wildcard Graphics_Systems/OpenGLES2/*.cpp) $(wildcard Graphics_Systems/OpenGL2/*.cpp) Graphics_Systems/OpenGLES2/glad/glad.c
SOURCES := $(filter-out Graphics_Systems/OpenGL2/GL2version.cpp, $(SOURCES))

VERTEX_SHADER_PREFIX := Graphics_Systems/OpenGL2/vertex_shader_prefix.glsl
VERTEX_SHADER := Graphics_Systems/OpenGL2/vertex_shader.glsl
FRAGMENT_SHADER_PREFIX := Graphics_Systems/OpenGL2/fragment_shader_prefix.glsl
FRAGMENT_SHADER := Graphics_Systems/OpenGL2/fragment_shader.glsl
