cd "${0%/*}"

if [ `uname` = "Darwin" ]; then
  clang++ "libdlgmod/general/libdlgmod.cpp" "libdlgmod/macos/libdlgmod.mm" -o "libdlgmod.dylib" -I. -std=c++17 -shared -ObjC++ -framework AppKit -framework UniformTypeIdentifiers -mmacos-version-min=10.13 -arch arm64 -arch x86_64 -fPIC;
elif [ $(uname) = "Linux" ]; then
  g++ "libdlgmod/general/libdlgmod.cpp" "libdlgmod/xlib/libdlgmod.cpp" "libdlgmod/general/apiprocess/process.cpp" "libdlgmod/general/xprocess.cpp" "libdlgmod/general/lodepng.cpp" -o "libdlgmod.so" -DPROCESS_GUIWINDOW_IMPL -DNULLIFY_STDERR -Ilibdlgmod/general -I. -std=c++17 -shared -static-libgcc -static-libstdc++ `pkg-config --cflags --libs x11` -lpthread -fPIC;
elif [ $(uname) = "FreeBSD" ]; then
  clang++ "libdlgmod/general/libdlgmod.cpp" "libdlgmod/xlib/libdlgmod.cpp" "libdlgmod/general/apiprocess/process.cpp" "libdlgmod/general/xprocess.cpp" "libdlgmod/general/lodepng.cpp" -o "libdlgmod.so" -DPROCESS_GUIWINDOW_IMPL -DNULLIFY_STDERR -Ilibdlgmod/general -I. -std=c++17 -shared `pkg-config --cflags --libs x11` -lkvm -lc -lpthread -fPIC;
elif [ $(uname) = "DragonFly" ]; then
  g++ "libdlgmod/general/libdlgmod.cpp" "libdlgmod/xlib/libdlgmod.cpp" "libdlgmod/general/apiprocess/process.cpp" "libdlgmod/general/xprocess.cpp" "libdlgmod/general/lodepng.cpp" -o "libdlgmod.so" -DPROCESS_GUIWINDOW_IMPL -DNULLIFY_STDERR -Ilibdlgmod/general -I. -std=c++17 -shared -static-libgcc -static-libstdc++ `pkg-config --cflags --libs x11` -lkvm -lc -lpthread -fPIC;
elif [ $(uname) = "NetBSD" ]; then
  g++ "libdlgmod/general/libdlgmod.cpp" "libdlgmod/xlib/libdlgmod.cpp" "libdlgmod/general/apiprocess/process.cpp" "libdlgmod/general/xprocess.cpp" "libdlgmod/general/lodepng.cpp" -o "libdlgmod.so" -DPROCESS_GUIWINDOW_IMPL -DNULLIFY_STDERR -Ilibdlgmod/general -I. -std=c++17 -shared -static-libgcc -static-libstdc++ `pkg-config --cflags --libs x11` -I/usr/X11R7/include -Wl,-rpath,/usr/X11R7/lib -L/usr/X11R7/lib -lkvm -lc -lpthread -fPIC;
elif [ $(uname) = "OpenBSD" ]; then
  clang++ "libdlgmod/general/libdlgmod.cpp" "libdlgmod/xlib/libdlgmod.cpp" "libdlgmod/general/apiprocess/process.cpp" "libdlgmod/general/xprocess.cpp" "libdlgmod/general/lodepng.cpp" -o "libdlgmod.so" -DPROCESS_GUIWINDOW_IMPL -DNULLIFY_STDERR -Ilibdlgmod/general -I. -std=c++17 -shared `pkg-config --cflags --libs x11` -lkvm -lc -lpthread -fPIC;
elif [ $(uname) = "SunOS" ]; then
  export PKG_CONFIG_PATH=/usr/lib/64/pkgconfig && g++ "libdlgmod/general/libdlgmod.cpp" "libdlgmod/xlib/libdlgmod.cpp" "libdlgmod/general/apiprocess/process.cpp" "libdlgmod/general/xprocess.cpp" "libdlgmod/general/lodepng.cpp" -o "libdlgmod.so" -DPROCESS_GUIWINDOW_IMPL -DNULLIFY_STDERR -Ilibdlgmod/general -I. -std=c++17 -shared -static-libgcc `pkg-config --cflags --libs x11` -lkvm -lc -lproc -lpthread -fPIC;
else
  g++ "libdlgmod/general/libdlgmod.cpp" "libdlgmod/win32/libdlgmod.cpp" "libdlgmod/general/apiprocess/process.cpp" "libdlgmod/general/xprocess.cpp" -o "libdlgmod.dll" -DPROCESS_GUIWINDOW_IMPL -DNULLIFY_STDERR -Ilibdlgmod/general -I. -std=c++17 -shared -static-libgcc -static-libstdc++ -static -lntdll -lgdiplus -lcomctl32 -lshlwapi -lcomdlg32 -lole32 -loleaut32 -luuid -fPIC;
fi
