SOURCES += $(wildcard Graphics_Systems/OpenGL1/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)
override LDLIBS += -lepoxy

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lopengl32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL
endif
