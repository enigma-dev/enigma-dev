SOURCES += $(wildcard Graphics_Systems/OpenGLES/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)
override CXXFLAGS += -IBridges/OpenGLES

override LDLIBS += -lepoxy

ifeq ($(TARGET_PLATFORM), Windows)
	override LDLIBS += -lopengl32
else ifeq ($(TARGET_PLATFORM), Linux)
	override LDLIBS += -lGL
else ifeq ($(TARGET_PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL
endif
