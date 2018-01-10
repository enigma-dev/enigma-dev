SOURCES += $(wildcard Graphics_Systems/OpenGL3/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)

LDLIBS += -lGLEW

ifeq ($(PLATFORM), Win32)
	LDLIBS += -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	LDLIBS += -lGL -lGLU
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenGL
endif


