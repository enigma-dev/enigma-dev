SHARED_SOURCES := $(SHARED_SOURCES) $(addprefix fonts/,$(call relative_wildcard,*.cpp))
override CXXFLAGS += $(shell pkg-config --cflags freetype2)
override LDLIBS += $(shell pkg-config --libs freetype2)
