override CXXFLAGS += -DXPROCESS_GUIWINDOW_IMPL
override CFLAGS += -DXPROCESS_GUIWINDOW_IMPL
ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Darwin)
		override LDLIBS += -framework CoreFoundation -framework CoreGraphics
	else ifeq ($(OS), Linux)
		override CXXFLAGS += $(shell pkg-config x11 --cflags)
		override CFLAGS += $(shell pkg-config x11 --cflags)
		override LDLIBS += $(shell pkg-config x11 --libs) -lprocps
	else ifeq ($(OS), FreeBSD)
		override CXXFLAGS += $(shell pkg-config x11 --cflags)
		override CFLAGS += $(shell pkg-config x11 --cflags)
		override LDLIBS += $(shell pkg-config x11 --libs) -lprocstat -lutil -lc
	endif
else
	CROSSPROCESS := $(shell "FILE=xProcess/crossprocess32.h;if test -f "$FILE"; then echo $FILE exists;else $(dirname $MINGW_PREFIX)/msys2_shell.cmd" -defterm -mingw32 -no-start -here -lc "g++ xProcess/crossprocess.cpp -o xProcess/crossprocess32.exe -std=c++17 -static-libgcc -static-libstdc++ -static -lShlwapi -m32";fi)
	CROSSPROCESS += $(shell "FILE=xProcess/crossprocess64.h;if test -f "$FILE"; then echo $FILE exists;else $(dirname $MINGW_PREFIX)/msys2_shell.cmd" -defterm -mingw64 -no-start -here -lc "g++ xProcess/crossprocess.cpp -o xProcess/crossprocess64.exe -std=c++17 -static-libgcc -static-libstdc++ -static -lShlwapi -m64";fi)
	CROSSPROCESS += $(shell FILE=xProcess/crossprocess32.h;if test -f "$FILE"; then echo $FILE exists;else cd xProcess/;xxd -i crossprocess32 > crossprocess32.h;fi)
	CROSSPROCESS += $(shell FILE=xProcess/crossprocess64.h;if test -f "$FILE"; then echo $FILE exists;else cd xProcess/;xxd -i crossprocess64 > crossprocess64.h;fi)
	CROSSPROCESS += $(shell rm -f "xProcess/crossprocess32.exe" "xProcess/crossprocess64.exe")
	override CXXFLAGS += -DXPROCESS_WIN32EXE_INCLUDES
	override CFLAGS += -DXPROCESS_WIN32EXE_INCLUDES
	override LDLIBS += -lShlwapi
endif
SHARED_SOURCES := $(SHARED_SOURCES) xProcess/crossprocess.cpp
