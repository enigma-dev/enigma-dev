override CXXFLAGS += -DXPROCESS_GUIWINDOW_IMPL
override CFLAGS += -DXPROCESS_GUIWINDOW_IMPL
ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Linux)
		override LDLIBS += -lprocps
	else ifeq ($(OS), FreeBSD)
		override LDLIBS += -lprocstat -lutil -lc
	endif
else
	CROSSPROCESS := $(shell "$(dirname $MINGW_PREFIX)/msys2_shell.cmd" -defterm -mingw32 -no-start -here -lc "g++ xProcess/crossprocess.cpp -o xProcess/crossprocess32.exe -std=c++17 -static-libgcc -static-libstdc++ -static -lShlwapi -m32")
	CROSSPROCESS += $(shell "$(dirname $MINGW_PREFIX)/msys2_shell.cmd" -defterm -mingw64 -no-start -here -lc "g++ xProcess/crossprocess.cpp -o xProcess/crossprocess64.exe -std=c++17 -static-libgcc -static-libstdc++ -static -lShlwapi -m64")
	CROSSPROCESS += $(shell cd xProcess/;xxd -i crossprocess32 > crossprocess32.h)
	CROSSPROCESS += $(shell cd xProcess/;xxd -i crossprocess64 > crossprocess64.h)
	CROSSPROCESS += $(shell rm -f "xProcess/crossprocess32.exe" "xProcess/crossprocess64.exe")
	override CXXFLAGS += -DXPROCESS_WIN32EXE_INCLUDES
	override CFLAGS += -DXPROCESS_WIN32EXE_INCLUDES
	override LDLIBS += -lShlwapi
endif
SHARED_SOURCES := $(SHARED_SOURCES) xProcess/crossprocess.cpp
