SOURCES += Universal_System/Extensions/VideoPlayer/videoplayer.cpp
ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -Wl,-Bdynamic `pkg-config --libs --cflags mpv` -Wl,-Bstatic
else ifeq ($(TARGET-PLATFORM), MacOSX)
	SOURCES += Universal_System/Extensions/VideoPlayer/videoplayer.mm
	override LDLIBS += -Wl,-Bdynamic `pkg-config --libs --cflags mpv` -Wl,-Bstatic
	override CXXFLAGS += -ObjC
	override CFLAGS += -ObjC
	override LDLIBS += -framework Cocoa
else ifeq ($(TARGET-PLATFORM), Android)
	override LDLIBS += -Wl,-Bdynamic `pkg-config --libs --cflags mpv` -Wl,-Bstatic
else # Linux, FreeBSD
	override LDLIBS += `pkg-config --libs --cflags mpv` # mostly-dynamic platforms
endif
