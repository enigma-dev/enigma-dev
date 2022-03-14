ifeq ($(TARGET-PLATFORM), Windows)
	override LDFLAGS += -lffi
	override LDLIBS += -lffi
	SOURCES += Universal_System/Extensions/ExternalFuncs/Win32/ExternalFuncs.cpp
else
	ifeq ($(TARGET-PLATFORM), MacOSX)
		override LDFLAGS += /opt/local/lib/libffi.a
		override LDLIBS += /opt/local/lib/libffi.a
	else
		override LDFLAGS += -lffi -ldl
		override LDLIBS += -lffi -ldl
	endif
	SOURCES += Universal_System/Extensions/ExternalFuncs/POSIX/ExternalFuncs.cpp
endif
SOURCES += Universal_System/Extensions/ExternalFuncs/ExternalFuncs.cpp
