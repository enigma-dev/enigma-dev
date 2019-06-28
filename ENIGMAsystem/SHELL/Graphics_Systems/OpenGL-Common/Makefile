override CXXFLAGS += -IGraphics_Systems/OpenGL-Common/

ifeq ($(GRAPHICS), OpenGL1)
	SOURCES += Graphics_Systems/OpenGL-Common/textures.cpp Graphics_Systems/OpenGL-Common/screen.cpp Graphics_Systems/OpenGL-Common/surface.cpp Graphics_Systems/OpenGL-Common/profiler.cpp
else
  SOURCES += $(wildcard Graphics_Systems/OpenGL-Common/*.cpp) 
endif
