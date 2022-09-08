SHARED_SRC_DIR := ../../shared
override CXXFLAGS += -I$(SHARED_SRC_DIR)/libpng-util
ifeq ($(UNIX_BASED), false)
	override LDLIBS += -lpng -lz
endif
SOURCES += Universal_System/Extensions/libpng/libpng-ext.cpp Universal_System/Extensions/libpng/libpng-util.cpp
