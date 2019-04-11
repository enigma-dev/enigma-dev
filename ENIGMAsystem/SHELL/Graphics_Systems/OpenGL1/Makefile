SOURCES += $(wildcard Graphics_Systems/OpenGL1/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)

ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += -DGLEW_STATIC
	override LDLIBS += -lglew32 -lopengl32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL -lGLEW
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL -lGLEW
endif
