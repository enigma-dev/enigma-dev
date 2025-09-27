override LDFLAGS += "/tmp/enigma.a"
override LDLIBS += "/tmp/enigma.a"
ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Darwin)
		override LDFLAGS += -framework OpenGL -lGLEW
		override LDLIBS += -framework OpenGL -lGLEW
	else
		ifeq ($(OS), Linux)
		else
			override LDFLAGS += -lc -lkvm
			override LDLIBS += -lc -lkvm
		endif
		override LDFLAGS += -lGL -lGLEW "../../shared/lunasvg/liblunasvg.a" "../../shared/lunasvg/plutovg/libplutovg.a" `pkg-config --libs x11` `pkg-config --libs --static sdl2` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -lX11 -lpthread -fPIC
		override LDLIBS += -lGL -lGLEW "../../shared/lunasvg/liblunasvg.a" "../../shared/lunasvg/plutovg/libplutovg.a" `pkg-config --libs x11` `pkg-config --libs --static sdl2` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -lX11 -lpthread -fPIC
		override CFLAGS += -I"../../shared/" `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
		override CXXFLAGS += -I"../../shared/" `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
	endif
else
	override CFLAGS += -DGLEW_STATIC
	override CXXFLAGS += -DGLEW_STATIC
	override LDFLAGS += -lglew32 -lopengl32 -lshell32 -lole32 -luuid
	override LDLIBS += -lglew32 -lopengl32 -lshell32 -lole32 -luuid
endif
override CFLAGS += -DIFD_SHARED_LIBRARY
override CXXFLAGS += -DIFD_SHARED_LIBRARY
override LDFLAGS += -Wl,-allow-multiple-definition
SOURCES += Widget_Systems/SDL/dialogs.cpp
IMGUIFD := $(shell Widget_Systems/SDL/imgui.sh)
