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
			override LDFLAGS += -lkvm
			override LDLIBS += -lkvm
		endif
		override LDFLAGS += -lGL
		override LDLIBS += -lGL
	endif
endif
override CFLAGS += -DCREATE_CONTEXT
override CXXFLAGS += -DCREATE_CONTEXT
SOURCES += Universal_System/Extensions/SystemInfo/system.cpp
