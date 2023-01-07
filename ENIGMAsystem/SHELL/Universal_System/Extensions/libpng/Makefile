SHARED_SRC_DIR := ../../shared
override CXXFLAGS += -I$(SHARED_SRC_DIR)/libpng-util

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lz -lpng
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lz -lpng
else ifeq ($(TARGET-PLATFORM), FreeBSD)
	override LDLIBS += -lz -lpng
else ifeq ($(TARGET-PLATFORM), DragonFlyBSD)
	override LDLIBS += -lz -lpng
else ifeq ($(TARGET-PLATFORM), OpenBSD)
	override LDLIBS += -lz -lpng
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += "/opt/local/lib/libz.a" "/opt/local/lib/libpng.a"
endif
SOURCES += Universal_System/Extensions/libpng/libpng-ext.cpp Universal_System/Extensions/libpng/libpng-util.cpp
