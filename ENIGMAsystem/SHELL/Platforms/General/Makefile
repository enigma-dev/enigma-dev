SHARED_SRC_DIR := ../../shared
SOURCES += $(SHARED_SRC_DIR)/__getexecname/internal.cpp
ifeq ($(TARGET-PLATFORM), Android)
	override CXXFLAGS += -IPlatforms/SDL/fileio
	override CFLAGS += -IPlatforms/SDL/fileio
	SOURCES += Platforms/SDL/fileio/fileio.cpp
else
	ifeq ($(TARGET-PLATFORM), Windows)
		override LDFLAGS += -lshell32 -lole32 -luuid
		override LDLIBS += -lshell32 -lole32 -luuid
	endif
	override CXXFLAGS += -IPlatforms/General/fileio
	override CFLAGS += -IPlatforms/General/fileio
	SOURCES += Platforms/General/fileio/fileio.cpp
endif
