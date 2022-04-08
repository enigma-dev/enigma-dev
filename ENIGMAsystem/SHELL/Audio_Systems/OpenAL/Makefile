SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDFLAGS += -framework OpenAL /usr/local/Cellar/alure/1.2_1/lib/libalure-static.a /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a
	override LDLIBS += -framework OpenAL /usr/local/Cellar/alure/1.2_1/lib/libalure-static.a /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a
else ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Linux)
		override LDLIBS += -lopenal -lalure -lvorbisfile -lvorbis -ldumb
	else
		override LDLIBS += -lopenal -lalure
	endif
else ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += $(shell pkg-config --cflags openal)
	override LDLIBS += -lALURE32-static -lopenal.dll -lvorbisfile -lvorbis -logg -lFLAC -lsndfile -lmodplug -lmpg123 -lole32 -lwinmm
endif
