#!/bin/sh
cd "${0%/*}/ENIGMAsystem/SHELL"

/bin/sh "./Widget_Systems/SDL/imgui.sh"
if [ `uname` = "Darwin" ]; then
  clang++ -c "../../emake.cpp" "../../shared/apiprocess/process.cpp" -std=c++17 `pkg-config --cflags sdl2`
  clang++ "/tmp/stigma.a" "emake.o" "process.o" -o "../../emake" -std=c++17 `pkg-config --libs sdl2` -lpthread -fPIC;
elif [ $(uname) = "Linux" ]; then
  g++ -c "../../emake.cpp" "../../shared/apiprocess/process.cpp" -std=c++17 `pkg-config --cflags sdl2` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0`
  g++ "/tmp/stigma.a" "../../shared/lunasvg/liblunasvg.a" "emake.o" "process.o" -o "../../emake" -std=c++17 `pkg-config --libs sdl2` `pkg-config --libs x11` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -static-libgcc -static-libstdc++ -lpthread -fPIC;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ -c "../../emake.cpp" "../../shared/apiprocess/process.cpp" -std=c++17 `pkg-config --cflags sdl2` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0`
  clang++ "/tmp/stigma.a" "../../shared/lunasvg/liblunasvg.a" "emake.o" "process.o" -o "../../emake" -std=c++17 `pkg-config --libs sdl2` `pkg-config --libs x11` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -lkvm -lc -lpthread -fPIC;
elif [ $(uname) = "DragonFly" ]; then
  g++ -c "../../emake.cpp" "../../shared/apiprocess/process.cpp" -std=c++17 `pkg-config --cflags sdl2` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0`
  g++ "/tmp/stigma.a" "../../shared/lunasvg/liblunasvg.a" "emake.o" "process.o" -o "../../emake" -std=c++17 `pkg-config --libs sdl2` `pkg-config --libs x11` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -static-libgcc -static-libstdc++ -lkvm -lc -lpthread -fPIC;
elif [ $(uname) = "NetBSD" ]; then
  g++ -c "../../emake.cpp" "../../shared/apiprocess/process.cpp" -std=c++17 `pkg-config --cflags sdl2` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0`
  g++ "/tmp/stigma.a" "../../shared/lunasvg/liblunasvg.a" "emake.o" "process.o" -o "../../emake" -std=c++17 `pkg-config --libs sdl2` `pkg-config --libs x11` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -static-libgcc -static-libstdc++ -lkvm -lc -lpthread -fPIC;
elif [ $(uname) = "OpenBSD" ]; then
  clang++ -c "../../emake.cpp" "../../shared/apiprocess/process.cpp" -std=c++17 `pkg-config --cflags sdl2` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0`
  clang++ "/tmp/stigma.a" "../../shared/lunasvg/liblunasvg.a" "emake.o" "process.o" -o "../../emake" -std=c++17 `pkg-config --libs sdl2` `pkg-config --libs x11` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -lkvm -lc -lpthread -fPIC;
elif [ $(uname) = "SunOS" ]; then
  g++ -c "../../emake.cpp" "../../shared/apiprocess/process.cpp" -std=c++17 `pkg-config --cflags sdl2` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0`
  export PKG_CONFIG_PATH=/usr/lib/64/pkgconfig && g++ "/tmp/stigma.a" "../../shared/lunasvg/liblunasvg.a" "emake.o" "process.o" -o "../../emake" -std=c++17 `pkg-config --libs sdl2` `pkg-config --libs x11` `pkg-config --libs gtk+-3.0` `pkg-config --libs gio-2.0` `pkg-config --libs glib-2.0` -static-libgcc -lkvm -lc -lproc -lpthread -fPIC;
else
  cd "${0%/*}"
  windres "Resources.rc" "Resources.o"
  g++ -c "emake.cpp" "shared/apiprocess/process.cpp" -std=c++17 `pkg-config --cflags sdl2 --static`
  g++ "/tmp/stigma.a" "emake.o" "process.o" "Resources.o" -o "emake.exe" -std=c++17 `pkg-config --libs sdl2 --static` -static -lshell32 -lole32 -luuid -lntdll -Wl,--subsystem,windows
fi
