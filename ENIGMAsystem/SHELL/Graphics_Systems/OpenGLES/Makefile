SOURCES += $(wildcard Graphics_Systems/OpenGLES/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)

ifeq ($(PLATFORM), Win32)
	override LDLIBS += -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	override LDLIBS += -lGL -lGLU
else ifeq ($(PLATFORM), Cocoa)
	override LDLIBS += -framework OpenGL
endif
