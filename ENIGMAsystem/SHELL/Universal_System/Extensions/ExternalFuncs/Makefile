ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lffi
	SOURCES += Universal_System/Extensions/ExternalFuncs/Win32/ExternalFuncs.cpp
else
	override LDLIBS += -lffi -ldl
	SOURCES += Universal_System/Extensions/ExternalFuncs/POSIX/ExternalFuncs.cpp
endif
SOURCES += Universal_System/Extensions/ExternalFuncs/ExternalFuncs.cpp
