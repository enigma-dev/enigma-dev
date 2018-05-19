SOURCES += $(wildcard Graphics_Systems/OpenGLES/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)

ifeq ($(PLATFORM), Win32)
	override CXXFLAGS += -DGLEW_STATIC
	override LDLIBS += -lglew32 -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	override LDLIBS += -lGL -lGLU -lGLEW
else ifeq ($(PLATFORM), Cocoa)
	override LDLIBS += -framework OpenGL -lGLEW
endif
