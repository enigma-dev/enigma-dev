ifeq ($(UNIX_BASED), false)
	override LDFLAGS += -lhwloc -lws2_32 -ldxgi
	override LDLIBS += -lhwloc -lws2_32 -ldxgi
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
		override LDFLAGS += -lGL -lhwloc
		override LDLIBS += -lGL -lhwloc
	endif
endif
SOURCES += Universal_System/Extensions/SystemInfo/system.cpp
