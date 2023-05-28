ifeq ($(UNIX_BASED), false)
	override LDFLAGS += -lws2_32 -lopengl32 -ld3d11 -ldxgi
	override LDLIBS += -lws2_32 -lopengl32 -ld3d11 -ldxgi
else
	ifeq ($(OS), Darwin)
		override LDFLAGS += -framework OpenGL
		override LDLIBS += -framework OpenGL
	else
		ifeq ($(OS), FreeBSD)
			override LDFLAGS += -lkvm
			override LDLIBS += -lkvm
		endif
		ifeq ($(OS), DragonFly)
			override LDFLAGS += -lkvm
			override LDLIBS += -lkvm
		endif
		override LDFLAGS += -lGL
		override LDLIBS += -lGL
	endif
endif
override CFLAGS += $(shell pkg-config --cflags glfw3)
override CXXFLAGS += $(shell pkg-config --cflags glfw3)
override LDFLAGS += $(shell pkg-config --libs glfw3)
override LDLIBS += $(shell pkg-config --libs glfw3)
SOURCES += Universal_System/Extensions/SystemInfo/system.cpp
