SOURCES += $(wildcard Graphics_Systems/DirectX10/*.cpp) $(wildcard Graphics_Systems/DirectX10/ParticleSystems/*.cpp)

ifeq ($(PLATFORM), Win32)
	LDLIBS += -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	LDLIBS += -lGL -lGLU
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenGL
endif
