SHARED_SRC_DIR := ../../shared
override CXXFLAGS += -I$(SHARED_SRC_DIR)/libpng-util
ifeq ($(OS), Darwin)
	override LDFLAGS += /usr/local/lib/libpng.a -lz
	override LDLIBS += /usr/local/lib/libpng.a -lz
else ifeq ($(OS), FreeBSD)
	override LDFLAGS +=  /usr/local/lib/libpng.a 
	override LDLIBS +=  /usr/local/lib/libpng.a 
else ifeq ($(OS), DragonFly)
	override LDFLAGS +=  /usr/local/lib/libpng.a 
	override LDLIBS +=  /usr/local/lib/libpng.a 
else ifeq ($(OS), OpenBSD)
	override LDFLAGS +=  /usr/local/lib/libpng.a 
	override LDLIBS +=  /usr/local/lib/libpng.a 
else
	override LDFLAGS += -lpng -lz
	override LDLIBS += -lpng -lz
endif

SOURCES += Universal_System/Extensions/libpng/libpng-ext.cpp Universal_System/Extensions/libpng/libpng-util.cpp
