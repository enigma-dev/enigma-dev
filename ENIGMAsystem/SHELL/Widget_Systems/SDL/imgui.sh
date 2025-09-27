#!/bin/sh
# build static library
export ENIGMA_PATH=`pwd`;
cd "/tmp"
if [ `uname` = "Darwin" ]; then
 clang++ -c "$ENIGMA_PATH/Widget_Systems/SDL/ImFileDialog/ImFileDialog.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_sdl2.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_opengl3.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_draw.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_tables.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_widgets.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/msgbox/imguial_msgbox.cpp" "$ENIGMA_PATH/../../shared/apifilesystem/filesystem.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/filedialogs.cpp" -std=c++17 -DIFD_SHARED_LIBRARY -I"$ENIGMA_PATH/../../shared/apifilesystem" -I"$ENIGMA_PATH/../../shared" -Wno-format-security -I"$ENIGMA_PATH" -I"$ENIGMA_PATH/Widget_Systems/SDL" -DIMGUI_USE_WCHAR32 -I/opt/local/include -I/opt/local/include/SDL2 -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -ObjC++ -fPIC
 ar rc "/tmp/enigma.a" "/tmp/ImFileDialog.o" "/tmp/imgui.o" "/tmp/imgui_impl_sdl2.o" "/tmp/imgui_impl_opengl3.o" "/tmp/imgui_draw.o" "/tmp/imgui_tables.o" "/tmp/imgui_widgets.o" "/tmp/imguial_msgbox.o" "/tmp/filesystem.o" "/tmp/filedialogs.o"
elif [ $(uname) = "Linux" ]; then
 cd "$ENIGMA_PATH/../../shared/lunasvg";
 rm -f "CMakeCache.txt";
 cmake .;
 make;
 cd "/tmp";
 g++ -c "$ENIGMA_PATH/Widget_Systems/SDL/ImFileDialog/ImFileDialog.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_sdl2.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_opengl3.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_draw.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_tables.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_widgets.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/msgbox/imguial_msgbox.cpp" "$ENIGMA_PATH/../../shared/apifilesystem/filesystem.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/filedialogs.cpp" -std=c++17 -DIFD_SHARED_LIBRARY -I"$ENIGMA_PATH/../../shared/apifilesystem" -I"$ENIGMA_PATH/../../shared" -Wno-format-security -I"$ENIGMA_PATH" -I"$ENIGMA_PATH/Widget_Systems/SDL" -DIMGUI_USE_WCHAR32 -I"$ENIGMA_PATH/../../shared/lunasvg/include/" `pkg-config --cflags sdl2 --static` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
 ar rc "/tmp/enigma.a"  "/tmp/ImFileDialog.o" "/tmp/imgui.o" "/tmp/imgui_impl_sdl2.o" "/tmp/imgui_impl_opengl3.o" "/tmp/imgui_draw.o" "/tmp/imgui_tables.o" "/tmp/imgui_widgets.o" "/tmp/imguial_msgbox.o" "/tmp/filesystem.o" "/tmp/filedialogs.o"
elif [ $(uname) = "FreeBSD" ]; then
 cd "$ENIGMA_PATH/../../shared/lunasvg";
 rm -f "CMakeCache.txt";
 cmake .;
 make;
 cd "/tmp";
 clang++ -c "$ENIGMA_PATH/Widget_Systems/SDL/ImFileDialog/ImFileDialog.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_sdl2.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_opengl3.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_draw.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_tables.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_widgets.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/msgbox/imguial_msgbox.cpp" "$ENIGMA_PATH/../../shared/apifilesystem/filesystem.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/filedialogs.cpp" -std=c++17 -DIFD_SHARED_LIBRARY -I"$ENIGMA_PATH/../../shared/apifilesystem" -I"$ENIGMA_PATH/../../shared" -Wno-format-security -I"$ENIGMA_PATH" -I"$ENIGMA_PATH/Widget_Systems/SDL" -DIMGUI_USE_WCHAR32 -I"$ENIGMA_PATH/../../shared/lunasvg/include/" `pkg-config --cflags sdl2 --static` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
  ar rc "/tmp/enigma.a"  "/tmp/ImFileDialog.o" "/tmp/imgui.o" "/tmp/imgui_impl_sdl2.o" "/tmp/imgui_impl_opengl3.o" "/tmp/imgui_draw.o" "/tmp/imgui_tables.o" "/tmp/imgui_widgets.o" "/tmp/imguial_msgbox.o" "/tmp/filesystem.o" "/tmp/filedialogs.o"
elif [ $(uname) = "DragonFly" ]; then
 cd "$ENIGMA_PATH/../../shared/lunasvg";
 rm -f "CMakeCache.txt";
 cmake .;
 make;
 cd "/tmp";
 g++ -c "$ENIGMA_PATH/Widget_Systems/SDL/ImFileDialog/ImFileDialog.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_sdl2.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_opengl3.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_draw.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_tables.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_widgets.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/msgbox/imguial_msgbox.cpp" "$ENIGMA_PATH/../../shared/apifilesystem/filesystem.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/filedialogs.cpp" -std=c++17 -DIFD_SHARED_LIBRARY -I"$ENIGMA_PATH/../../shared/apifilesystem" -I"$ENIGMA_PATH/../../shared" -Wno-format-security -I"$ENIGMA_PATH" -I"$ENIGMA_PATH/Widget_Systems/SDL" -DIMGUI_USE_WCHAR32 -I"$ENIGMA_PATH/../../shared/lunasvg/include/" `pkg-config --cflags sdl2 --static` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
  ar rc "/tmp/enigma.a"  "/tmp/ImFileDialog.o" "/tmp/imgui.o" "/tmp/imgui_impl_sdl2.o" "/tmp/imgui_impl_opengl3.o" "/tmp/imgui_draw.o" "/tmp/imgui_tables.o" "/tmp/imgui_widgets.o" "/tmp/imguial_msgbox.o" "/tmp/filesystem.o" "/tmp/filedialogs.o"
elif [ $(uname) = "NetBSD" ]; then
 cd "$ENIGMA_PATH/../../shared/lunasvg";
 rm -f "CMakeCache.txt";
 cmake .;
 make;
 cd "/tmp";
 g++ -c "$ENIGMA_PATH/Widget_Systems/SDL/ImFileDialog/ImFileDialog.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_sdl2.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_opengl3.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_draw.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_tables.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_widgets.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/msgbox/imguial_msgbox.cpp" "$ENIGMA_PATH/../../shared/apifilesystem/filesystem.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/filedialogs.cpp" -std=c++17 -DIFD_SHARED_LIBRARY -I"$ENIGMA_PATH/../../shared/apifilesystem" -I"$ENIGMA_PATH/../../shared" -Wno-format-security -I"$ENIGMA_PATH" -I"$ENIGMA_PATH/Widget_Systems/SDL" -DIMGUI_USE_WCHAR32 -I"$ENIGMA_PATH/../../shared/lunasvg/include/" `pkg-config --cflags sdl2 --static` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
  ar rc "/tmp/enigma.a"  "/tmp/ImFileDialog.o" "/tmp/imgui.o" "/tmp/imgui_impl_sdl2.o" "/tmp/imgui_impl_opengl3.o" "/tmp/imgui_draw.o" "/tmp/imgui_tables.o" "/tmp/imgui_widgets.o" "/tmp/imguial_msgbox.o" "/tmp/filesystem.o" "/tmp/filedialogs.o"
elif [ $(uname) = "OpenBSD" ]; then
 cd "$ENIGMA_PATH/../../shared/lunasvg";
 rm -f "CMakeCache.txt";
 cmake .;
 make;
 cd "/tmp";
 clang++ -c "$ENIGMA_PATH/Widget_Systems/SDL/ImFileDialog/ImFileDialog.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_sdl2.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_opengl3.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_draw.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_tables.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_widgets.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/msgbox/imguial_msgbox.cpp" "$ENIGMA_PATH/../../shared/apifilesystem/filesystem.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/filedialogs.cpp" -std=c++17 -DIFD_SHARED_LIBRARY -I"$ENIGMA_PATH/../../shared/apifilesystem" -I"$ENIGMA_PATH/../../shared" -Wno-format-security -I"$ENIGMA_PATH" -I"$ENIGMA_PATH/Widget_Systems/SDL" -DIMGUI_USE_WCHAR32 -I"$ENIGMA_PATH/../../shared/lunasvg/include/" `pkg-config --cflags sdl2 --static` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
 ar rc "/tmp/enigma.a"  "/tmp/ImFileDialog.o" "/tmp/imgui.o" "/tmp/imgui_impl_sdl2.o" "/tmp/imgui_impl_opengl3.o" "/tmp/imgui_draw.o" "/tmp/imgui_tables.o" "/tmp/imgui_widgets.o" "/tmp/imguial_msgbox.o" "/tmp/filesystem.o" "/tmp/filedialogs.o"
elif [ $(uname) = "SunOS" ]; then
 cd "$ENIGMA_PATH/../../shared/lunasvg";
 rm -f "CMakeCache.txt";
 cmake .;
 make;
 cd "/tmp";
 export PKG_CONFIG_PATH=/usr/lib/64/pkgconfig && g++ -c "$ENIGMA_PATH/Widget_Systems/SDL/ImFileDialog/ImFileDialog.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_sdl2.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_opengl3.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_draw.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_tables.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_widgets.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/msgbox/imguial_msgbox.cpp" "$ENIGMA_PATH/../../shared/apifilesystem/filesystem.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/filedialogs.cpp" -std=c++17 -DIFD_SHARED_LIBRARY -I"$ENIGMA_PATH/../../shared/apifilesystem" -I"$ENIGMA_PATH/../../shared" -Wno-format-security -I"$ENIGMA_PATH" -I"$ENIGMA_PATH/Widget_Systems/SDL" -DIMGUI_USE_WCHAR32 -I"$ENIGMA_PATH/../../shared/lunasvg/include/" `pkg-config --cflags sdl2 --static` `pkg-config --cflags x11` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gio-2.0` `pkg-config --cflags glib-2.0` -fPIC
 ar rc "/tmp/enigma.a" "/tmp/ImFileDialog.o" "/tmp/imgui.o" "/tmp/imgui_impl_sdl2.o" "/tmp/imgui_impl_opengl3.o" "/tmp/imgui_draw.o" "/tmp/imgui_tables.o" "/tmp/imgui_widgets.o" "/tmp/imguial_msgbox.o" "/tmp/filesystem.o" "/tmp/filedialogs.o"
else
 g++ -c "$ENIGMA_PATH/Widget_Systems/SDL/ImFileDialog/ImFileDialog.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_sdl2.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_impl_opengl3.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_draw.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_tables.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/imgui/imgui_widgets.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/msgbox/imguial_msgbox.cpp" "$ENIGMA_PATH/../../shared/apifilesystem/filesystem.cpp" "$ENIGMA_PATH/Widget_Systems/SDL/filedialogs.cpp" -std=c++17 -DIFD_SHARED_LIBRARY -I"$ENIGMA_PATH/../../shared/apifilesystem" -I"$ENIGMA_PATH/../../shared" -I"$ENIGMA_PATH" -I"$ENIGMA_PATH/Widget_Systems/SDL" -D_UNICODE -DUNICODE -DIMGUI_USE_WCHAR32 `pkg-config --cflags sdl2 --static` -static-libgcc -static-libstdc++ -static -fPIC
 ar rc "/tmp/enigma.a" "/tmp/ImFileDialog.o" "/tmp/imgui.o" "/tmp/imgui_impl_sdl2.o" "/tmp/imgui_impl_opengl3.o" "/tmp/imgui_draw.o" "/tmp/imgui_tables.o" "/tmp/imgui_widgets.o" "/tmp/imguial_msgbox.o" "/tmp/filesystem.o" "/tmp/filedialogs.o"
fi
