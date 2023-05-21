ifeq ($(UNIX_LIKE), false)
	override LDFLAGS += -lws2_32 -lopengl32
	override LDLIBS += -lws2_32 -lopengl32
else
	ifeq ($(OS), Darwin)
		override LDFLAGS += -framework OpenGL
		override LDLIBS += -framework OpenGL
	else
		override LDFLAGS += -lGL	
		override LDLIBS += -lGL
	endif
endif
SOURCES += Universal_System/Extensions/ShellExecution/sysinfo.cpp
