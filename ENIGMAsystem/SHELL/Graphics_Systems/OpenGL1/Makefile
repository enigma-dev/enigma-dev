SOURCES += $(wildcard Graphics_Systems/OpenGL1/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)

ifeq ($(PLATFORM), Win32)
	LDLIBS += -lopengl32 -lglu32 -lglew32
else ifeq ($(PLATFORM), xlib)
	LDLIBS += -lGL -lGLU -lGLEW
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenGL -lGLEW
endif
