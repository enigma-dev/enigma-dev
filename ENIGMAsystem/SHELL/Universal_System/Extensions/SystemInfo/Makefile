ifeq ($(UNIX_BASED), false)
	override LDFLAGS += -lws2_32 -lopengl32 -ld3d11 -ldxgi $(shell pkg-config --cflags --libs glfw3)
	override LDLIBS += -lws2_32 -lopengl32 -ld3d11 -ldxgi $(shell pkg-config --cflags --libs glfw3)
else
	ifeq ($(OS), Darwin)
		override LDFLAGS += -framework OpenGL $(shell pkg-config --cflags --libs glfw3)
		override LDLIBS += -framework OpenGL $(shell pkg-config --cflags --libs glfw3)
	else
		ifeq ($(OS), FreeBSD)
			override LDFLAGS += -lkvm
			override LDLIBS += -lkvm
		endif
		ifeq ($(OS), DragonFly)
			override LDFLAGS += -lkvm
			override LDLIBS += -lkvm
		endif
		override LDFLAGS += -lGL $(shell pkg-config --cflags --libs glfw3)	
		override LDLIBS += -lGL $(shell pkg-config --cflags --libs glfw3)
	endif
endif
SOURCES += Universal_System/Extensions/SystemInfo/system.cpp
