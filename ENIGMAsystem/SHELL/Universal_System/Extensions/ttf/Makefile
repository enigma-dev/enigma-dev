SOURCES +=  $(wildcard Universal_System/Extensions/ttf/*.cpp)
ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDFLAGS += /opt/local/lib/libfreetype.a
	override LDLIBS += /opt/local/lib/libfreetype.a
else ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += $(shell pkg-config --cflags freetype2)
        FREETYPE := $(shell echo "${MSYSTEM_PREFIX}/lib/libfreetype.dll.a")
	override LDFLAGS += $(FREETYPE)
	override LDLIBS += $(FREETYPE)
else
	override CXXFLAGS += $(shell pkg-config --cflags freetype2)
	override LDLIBS += $(shell pkg-config --libs freetype2)
endif
