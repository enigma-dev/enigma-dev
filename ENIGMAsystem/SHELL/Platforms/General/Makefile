SHARED_SRC_DIR := ../../shared
override CXXFLAGS += -I$(SHARED_SRC_DIR)/__getexecname
override CFLAGS += -I$(SHARED_SRC_DIR)/__getexecname
SOURCES += $(SHARED_SRC_DIR)/__getexecname/internal.cpp
ifeq ($(TARGET-PLATFORM), Android)
	override CXXFLAGS += -IPlatforms/SDL/fileio
	override CFLAGS += -IPlatforms/SDL/fileio
	SOURCES += Platforms/SDL/fileio/fileio.cpp
else
	override CXXFLAGS += -IPlatforms/General/fileio
	override CFLAGS += -IPlatforms/General/fileio
	SOURCES += Platforms/General/fileio/fileio.cpp
endif
