SOURCES += $(wildcard Graphics_Systems/OpenGL/*.cpp)

ifeq ($(PLATFORM), Win32)
	LDLIBS += ../Additional/glew/lib/glew32s.lib -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	LDLIBS += -lGL -lGLU
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenGL
endif