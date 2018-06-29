CODECS := vorbisfile vorbis ogg flac sndfile libmodplug libmpg123
SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(TARGET-PLATFORM), Linux)
	override CXXFLAGS += $(shell pkg-config --cflags openal alure $(CODECS))
	override LDLIBS += $(shell pkg-config --libs openal alure $(CODECS))
else
	override CXXFLAGS += $(shell pkg-config --cflags --static openal alure $(CODECS))
	#unsure if pkgconfig sets this on windows or not
	override CXXFLAGS += -DALURE_STATIC_LIBRARY
	override LDLIBS += $(shell pkg-config --libs --static openal alure $(CODECS))
endif

#pkgconf for dumb incorrectly list dumb's lib as -llibdumb. move dumb to codecs if it's ever fixed
override LDLIBS += -ldumb
