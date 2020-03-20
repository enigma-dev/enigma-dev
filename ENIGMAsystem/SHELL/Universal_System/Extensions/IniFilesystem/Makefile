OS := $(shell uname -s)
ifeq ($(OS), Linux)
	SOURCES += Universal_System/Extensions/IniFilesystem/UNIXiniindex.cpp
	SOURCES += Universal_System/Extensions/IniFilesystem/UNIXinifs.cpp
else ifeq ($(OS), Darwin)
	SOURCES += Universal_System/Extensions/IniFilesystem/UNIXiniindex.cpp
	SOURCES += Universal_System/Extensions/IniFilesystem/UNIXinifs.cpp
else
	SOURCES += Universal_System/Extensions/IniFilesystem/WINDOWSinifs.cpp
endif
