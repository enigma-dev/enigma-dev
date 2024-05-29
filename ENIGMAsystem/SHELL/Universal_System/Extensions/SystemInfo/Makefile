ifeq ($(UNIX_BASED), false)
	override LDFLAGS += $(shell pkg-config --libs hwloc --static) -lws2_32 -ldxgi
	override LDLIBS += $(shell pkg-config --libs hwloc --static) -lws2_32 -ldxgi
	override CFLAGS += $(shell pkg-config --cflags hwloc)
	override CXXFLAGS += $(shell pkg-config --cflags hwloc)
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
		override LDFLAGS += -lGL $(shell pkg-config --libs hwloc --static)
		override LDLIBS += -lGL $(shell pkg-config --libs hwloc --static)
		override CFLAGS += $(shell pkg-config --cflags hwloc)
		override CXXFLAGS += $(shell pkg-config --cflags hwloc)
	endif
endif
SOURCES += Universal_System/Extensions/SystemInfo/system.cpp
