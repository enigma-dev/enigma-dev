ifeq ($(UNIX_BASED), false)
	override LDFLAGS += -lws2_32 -ldxgi
	override LDLIBS += -lws2_32 -ldxgi
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
			override LDFLAGS += $(shell pkg-config --libs hwloc --static) -lkvm
			override LDLIBS += $(shell pkg-config --libs hwloc --static) -lkvm
			override CFLAGS += $(shell pkg-config --cflags hwloc)
			override CXXFLAGS += $(shell pkg-config --cflags hwloc)
		endif
		override LDFLAGS += -lGL
		override LDLIBS += -lGL
	endif
endif
SOURCES += Universal_System/Extensions/SystemInfo/system.cpp
