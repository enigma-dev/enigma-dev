SOURCES +=  $(wildcard Universal_System/Extensions/ttf/*.cpp)
ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDFLAGS += /opt/local/lib/libfreetype.a
	override LDLIBS += /opt/local/lib/libfreetype.a
else
	override CXXFLAGS += $(shell pkg-config --cflags freetype2)
	override LDLIBS += $(shell pkg-config --libs --static freetype2) -lfreetype
endif
