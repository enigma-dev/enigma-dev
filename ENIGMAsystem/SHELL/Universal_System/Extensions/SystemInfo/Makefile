ifeq ($(UNIX_BASED), false)
	override LDFLAGS += -lws2_32 -lopengl32 -ld3d11 -ldxgi
	override LDLIBS += -lws2_32 -lopengl32 -ld3d11 -ldxgi
else
	ifeq ($(OS), Darwin)
		override LDFLAGS += -framework OpenGL
		override LDLIBS += -framework OpenGL
	else
		ifeq ($(OS), FreeBSD)
			override LDFLAGS += -lsysinfo
			override LDLIBS += -lsysinfo
		endif
		ifeq ($(OS), DragonFly)
			override LDFLAGS += -lsysinfo	
			override LDLIBS += -lsysinfo
		endif
		override LDFLAGS += -lGL	
		override LDLIBS += -lGL
	endif
endif
SOURCES += Universal_System/Extensions/SystemInfo/system.cpp
