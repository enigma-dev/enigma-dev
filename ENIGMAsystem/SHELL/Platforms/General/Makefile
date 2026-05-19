SHARED_SRC_DIR := ../../shared
override CXXFLAGS += -I$(SHARED_SRC_DIR)/__getexecname
override CFLAGS += -I$(SHARED_SRC_DIR)/__getexecname
override CXXFLAGS += -I$(SHARED_SRC_DIR)/__getbasepath
override CFLAGS += -I$(SHARED_SRC_DIR)/__getbasepath
override CXXFLAGS += -I$(SHARED_SRC_DIR)/__getprogname
override CFLAGS += -I$(SHARED_SRC_DIR)/__getprogname
SOURCES += $(SHARED_SRC_DIR)/__getexecname/internal.cpp
SOURCES += $(SHARED_SRC_DIR)/__getbasepath/internal.cpp
SOURCES += $(SHARED_SRC_DIR)/__getprogname/internal.cpp
ifeq ($(TARGET-PLATFORM), Android)
	override CXXFLAGS += -IPlatforms/SDL/fileio
	override CFLAGS += -IPlatforms/SDL/fileio
	SOURCES += Platforms/SDL/fileio/fileio.cpp
else
	override CXXFLAGS += -IPlatforms/General/fileio
	override CFLAGS += -IPlatforms/General/fileio
	SOURCES += Platforms/General/fileio/fileio.cpp
endif
