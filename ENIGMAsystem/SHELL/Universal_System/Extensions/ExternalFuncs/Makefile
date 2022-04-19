ifeq ($(TARGET-PLATFORM), Windows)
	override LDFLAGS += -lffi
	override LDLIBS += -lffi
	SOURCES += Universal_System/Extensions/ExternalFuncs/Win32/ExternalFuncs.cpp
else
	ifeq ($(TARGET-PLATFORM), MacOSX)
		override LDFLAGS += /usr/local/opt/libffi/lib/libffi.a
		override LDLIBS += /usr/local/opt/libffi/lib/libffi.a
		override CXXFLAGS += -I/usr/local/opt/libffi/include
		override CFLAGS += -I/usr/local/opt/libffi/include
	else ifeq ($(OS), FreeBSD)
		override LDFLAGS += /usr/local/lib/libffi.a
		override LDLIBS += /usr/local/lib/libffi.a
	else ifeq ($(OS), DragonFly)
		override LDFLAGS += /usr/local/lib/libffi.a
		override LDLIBS += /usr/local/lib/libffi.a
	else ifeq ($(OS), OpenBSD)
		override LDFLAGS += /usr/local/lib/libffi.a
		override LDLIBS += /usr/local/lib/libffi.a
	else
		override LDFLAGS += -lffi -ldl
		override LDLIBS += -lffi -ldl
	endif
	SOURCES += Universal_System/Extensions/ExternalFuncs/POSIX/ExternalFuncs.cpp
endif
SOURCES += Universal_System/Extensions/ExternalFuncs/ExternalFuncs.cpp
