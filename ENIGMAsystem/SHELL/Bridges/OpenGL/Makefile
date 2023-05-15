ifeq ($(shell uname -s), Darwin)
override CFLAGS += -I/opt/local/include
override CXXFLAGS += -I/opt/local/include
override LDLIBS += -L/opt/local/lib -framework OpenGL /opt/local/lib/libGLEW.a
override LDFLAGS += -L/opt/local/lib -framework OpenGL /opt/local/lib/libGLEW.a
endif
SOURCES += $(wildcard Bridges/OpenGL/*.cpp)
