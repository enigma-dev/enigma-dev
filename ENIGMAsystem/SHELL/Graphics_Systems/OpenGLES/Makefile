SOURCES += $(wildcard Graphics_Systems/OpenGLES/*.cpp) $(wildcard Graphics_Systems/General/*.cpp)
override CXXFLAGS += -IBridges/OpenGLES -I../../android/external/glad/gles2/include

#override LDLIBS += -lepoxy

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lopengl32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL
endif
