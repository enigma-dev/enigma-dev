ifeq ($(OS), Linux)
	SOURCES += Universal_System/Extensions/VideoPlayer/Universal/insecure_bullshit.cpp
	SOURCES += Universal_System/Extensions/VideoPlayer/Linux/insecure_bullshit.cpp
	SOURCES += Universal_System/Extensions/VideoPlayer/POSIX/insecure_bullshit.cpp
	SOURCES += Universal_System/Extensions/VideoPlayer/POSIX/videoplayer.cpp
	override LDLIBS += -lprocps
else ifeq ($(OS), FreeBSD)
	SOURCES += Universal_System/Extensions/VideoPlayer/Universal/insecure_bullshit.cpp
	SOURCES += Universal_System/Extensions/VideoPlayer/FreeBSD/insecure_bullshit.cpp
	SOURCES += Universal_System/Extensions/VideoPlayer/POSIX/insecure_bullshit.cpp
	SOURCES += Universal_System/Extensions/VideoPlayer/POSIX/videoplayer.cpp
	override LDLIBS += -lc -lutil -lprocstat
else
	SOURCES += Universal_System/Extensions/VideoPlayer/Win32/insecure_bullshit.cpp
	SOURCES += Universal_System/Extensions/VideoPlayer/Win32/videoplayer.cpp
	override CFLAGS += -DWINBLOWS
	override CXXFLAGS += -DWINBLOWS
	override LDLIBS += -lole32 -lstrmiids
endif
