SOURCES += Universal_System/Extensions/VideoPlayer/videoplayer.cpp
ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -Wl,-Bdynamic `pkg-config --libs --cflags mpv` -Wl,-Bstatic
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -Wl,-Bdynamic `pkg-config --libs --cflags mpv` -Wl,-Bstatic
else ifneq ($(TARGET-PLATFORM), Android)
	override LDLIBS += -Wl,-Bdynamic `pkg-config --libs --cflags mpv` -Wl,-Bstatic
else # Linux, FreeBSD
	# FreeBSD has not been added yet in this pr so I can't do that check right now
	# that's why I have a check for Windows, MacOSX, Android instead of Linux/else
	override LDLIBS += `pkg-config --libs --cflags mpv` # mostly-dynamic platforms
endif