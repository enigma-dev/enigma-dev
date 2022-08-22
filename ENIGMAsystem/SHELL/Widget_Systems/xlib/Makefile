
SHARED_SRC_DIR := ../../shared
override CXXFLAGS += -I$(SHARED_SRC_DIR)/lib -std=c++17
override CFLAGS += -I$(SHARED_SRC_DIR)/lib -std=c++17
ifeq ($(OS), Linux)
	override LDFLAGS += -lz -lpng
	override LDLIBS += -lz -lpng
endif
SOURCES += $(wildcard Widget_Systems/xlib/*.cpp) Widget_Systems/General/WSdialogs.cpp
