ifeq ($(OS), Linux)
	SOURCES += Universal_System/Extensions/VideoPlayer/Linux/insecure_bullshit.cpp
	SOURCES += Universal_System/Extensions/VideoPlayer/POSIX/insecure_bullshit.cpp
	override CFLAGS += -DUNIX_LIKE=1
	override CXXFLAGS += -DUNIX_LIKE=1
	override LDLIBS += -lprocps
else ifeq ($(OS), FreeBSD)
	SOURCES += Universal_System/Extensions/VideoPlayer/FreeBSD/insecure_bullshit.cpp
	SOURCES += Universal_System/Extensions/VideoPlayer/POSIX/insecure_bullshit.cpp
	override CFLAGS += -DUNIX_LIKE=1
	override CXXFLAGS += -DUNIX_LIKE=1
	override LDLIBS += -lc -lutil -lprocstat
else
	SOURCES += Universal_System/Extensions/VideoPlayer/Win32/insecure_bullshit.cpp
	override CFLAGS += -DUNIX_LIKE=0
	override CXXFLAGS += -DUNIX_LIKE=0
	override LDLIBS += -lole32
endif
SOURCES += Universal_System/Extensions/VideoPlayer/Universal/insecure_bullshit.cpp
SOURCES += Universal_System/Extensions/VideoPlayer/videoplayer.cpp
