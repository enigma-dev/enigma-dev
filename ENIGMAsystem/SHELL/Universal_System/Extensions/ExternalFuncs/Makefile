ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += -IUniversal_System/Extensions/ExternalFuncs/Win32/
	override CFLAGS += -IUniversal_System/Extensions/ExternalFuncs/Win32/
	override LDLIBS += -lffi
	SOURCES += Universal_System/Extensions/ExternalFuncs/Win32/ExternalFuncs.cpp
else
	override CXXFLAGS += -IUniversal_System/Extensions/ExternalFuncs/POSIX/
	override CFLAGS += -IUniversal_System/Extensions/ExternalFuncs/POSIX/
	override LDLIBS += -lffi -ldl
	SOURCES += Universal_System/Extensions/ExternalFuncs/POSIX/ExternalFuncs.cpp
endif
SOURCES += Universal_System/Extensions/ExternalFuncs/ExternalFuncs.cpp
