override LDFLAGS += "/tmp/stigma.a"
override LDLIBS += "/tmp/stigma.a"
ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Darwin)
	else
		ifeq ($(OS), Linux)
		else
			override LDFLAGS += -lc -lkvm
			override LDLIBS += -lc -lkvm
		endif
		override LDFLAGS += "../../shared/lunasvg/liblunasvg.a" "../../shared/lunasvg/plutovg/libplutovg.a" `pkg-config --libs x11` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -lX11 -lpthread -fPIC
		override LDLIBS += "../../shared/lunasvg/liblunasvg.a" "../../shared/lunasvg/plutovg/libplutovg.a" `pkg-config --libs x11` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -lX11 -lpthread -fPIC
		override CFLAGS += -I"../../shared/" -I"../../shared/lunasvg/include/" `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
		override CXXFLAGS += -I"../../shared/" -I"../../shared/lunasvg/include/" `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
	endif
else
	override LDFLAGS += -lshell32 -lole32 -luuid
	override LDLIBS += -lshell32 -lole32 -luuid
endif
override CFLAGS += -DIFD_SHARED_LIBRARY
override CXXFLAGS += -DIFD_SHARED_LIBRARY
override LDFLAGS += -Wl,-allow-multiple-definition
SOURCES += Widget_Systems/SDL/dialogs.cpp
IMGUIFD := $(shell Widget_Systems/SDL/imgui.sh)
