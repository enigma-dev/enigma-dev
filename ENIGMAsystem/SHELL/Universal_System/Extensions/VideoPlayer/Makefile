SOURCES += Universal_System/Extensions/VideoPlayer/videoplayer.cpp
ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -Wl,-Bdynamic -lmpv -Wl,-Bstatic
else ifeq ($(TARGET-PLATFORM), MacOSX)
	SOURCES += Universal_System/Extensions/VideoPlayer/videoplayer.mm
	override CXXFLAGS += $(shell $(PKG_CONFIG) --cflags mpv)
	override LDLIBS += -Wl,-Bdynamic $(shell $(PKG_CONFIG) --libs mpv) -Wl,-Bstatic
	override CXXFLAGS += -ObjC
	override CFLAGS += -ObjC
	override LDLIBS += -framework Cocoa
else ifeq ($(TARGET-PLATFORM), Android)
	override CXXFLAGS += $(shell $(PKG_CONFIG) --cflags mpv)
	override LDLIBS += -Wl,-Bdynamic $(shell $(PKG_CONFIG) --libs mpv) -Wl,-Bstatic
else # Linux, FreeBSD
	override CXXFLAGS += $(shell $(PKG_CONFIG) --cflags mpv)
	override LDLIBS += $(shell $(PKG_CONFIG) --libs mpv)
endif
