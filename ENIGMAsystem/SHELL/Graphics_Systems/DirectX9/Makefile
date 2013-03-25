SOURCES += $(wildcard Graphics_Systems/DirectX9/*.cpp) Graphics_Systems/OpenGL3/glew.c $(wildcard Graphics_Systems/DirectX9/ParticleSystems/*.cpp)

ifeq ($(PLATFORM), Win32)
	LDLIBS += -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	LDLIBS += -lGL -lGLU
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenGL
endif
