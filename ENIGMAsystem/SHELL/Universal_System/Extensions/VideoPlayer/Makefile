SOURCES += Universal_System/Extensions/VideoPlayer/videoplayer.cpp
ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += $(shell $(pkg-config) --cflags mpv)
	override LDLIBS += -Wl,-Bdynamic $(shell $(pkg-config) --libs mpv) -Wl,-Bstatic
	override LDLIBS += `pkg-config --libs --cflags mpv`
else ifeq ($(TARGET-PLATFORM), MacOSX)
	SOURCES += Universal_System/Extensions/VideoPlayer/videoplayer.mm
	override CXXFLAGS += $(shell $(pkg-config) --cflags mpv)
	override LDLIBS += -Wl,-Bdynamic $(shell $(pkg-config) --libs mpv) -Wl,-Bstatic
	override CXXFLAGS += -ObjC
	override CFLAGS += -ObjC
	override LDLIBS += -framework Cocoa
else ifeq ($(TARGET-PLATFORM), Android)
	override CXXFLAGS += $(shell $(pkg-config) --cflags mpv)
	override LDLIBS += -Wl,-Bdynamic $(shell $(pkg-config) --libs mpv) -Wl,-Bstatic
else # Linux, FreeBSD
	override CXXFLAGS += $(shell $(pkg-config) --cflags mpv)
	override LDLIBS += $(shell $(pkg-config) --libs mpv)
endif
