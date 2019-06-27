OS := $(shell uname -s)
ifeq ($(OS), Linux)
	SOURCES += $(wildcard Universal_System/Extensions/IniFilesystem/IniFileIndex.cpp)
	SOURCES += $(wildcard Universal_System/Extensions/IniFilesystem/inifilesystem.cpp)
else ifeq ($(OS), Darwin)
	SOURCES += $(wildcard Universal_System/Extensions/IniFilesystem/IniFileIndex.cpp)
	SOURCES += $(wildcard Universal_System/Extensions/IniFilesystem/inifilesystem.cpp)
else
	SOURCES += $(wildcard Universal_System/Extensions/IniFilesystem/Win32Ini.cpp)
endif
