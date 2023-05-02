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
		override LDFLAGS +=  -L"../../shared/lunasvg" -llunasvg `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -lX11 -lpthread -fPIC
		override LDLIBS +=  -L"../../shared/lunasvg" -llunasvg `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -lX11 -lpthread -fPIC
		override CFLAGS += -I"../../shared/" `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
		override CXXFLAGS += -I"../../shared/" `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
	endif
else
	override LDFLAGS += -lshell32 -lole32 -luuid
	override LDFLAGS += -lshell32 -lole32 -luuid
endif
SOURCES += Widget_Systems/SDL/dialogs.cpp
IMGUIFD := $(shell Widget_Systems/SDL/imgui.sh)
