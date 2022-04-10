SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDFLAGS += -framework OpenAL /usr/local/Cellar/alure/1.2_1/lib/libalure-static.a /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a /opt/local/lib/libdumb.a /opt/local/lib/libsndfile.a
	override LDLIBS += -framework OpenAL /usr/local/Cellar/alure/1.2_1/lib/libalure-static.a /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a /opt/local/lib/libdumb.a /opt/local/lib/libsndfile.a
else ifeq ($(UNIX_BASED), true)
	ifeq ($(TARGET-PLATFORM), Linux)
		override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbis -logg -lFLAC -lopus -lopusfile -ldumb -lsndfile -lmodplug -lmpg123 -lssp
	else
		override LDLIBS += -lalure -lopenal  /usr/local/lib/libvorbis.a /usr/local/lib/libogg.a /usr/local/lib/libvorbisfile.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libFLAC.a /usr/local/lib/libopus.a /usr/local/lib/libopusfile.a  /usr/local/lib/libdumb.a -lmodplug -lmpg123 -lfluidsynth -lssp
	endif
else ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += $(shell pkg-config --cflags openal)
	override LDLIBS += -lALURE32-static -lopenal.dll -lvorbisfile -lvorbis -logg -lFLAC -lopus -lopusfile -ldumb -lsndfile -lmodplug -lmpg123 -lssp -lole32 -lwinmm
endif
