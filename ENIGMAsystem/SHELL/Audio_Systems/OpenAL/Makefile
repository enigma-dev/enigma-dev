SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDFLAGS += -framework OpenAL /usr/local/Cellar/alure/1.2_1/lib/libalure-static.a /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a /opt/local/lib/libdumb.a /opt/local/lib/libsndfile.a
	override LDLIBS += -framework OpenAL /usr/local/Cellar/alure/1.2_1/lib/libalure-static.a /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a /opt/local/lib/libdumb.a /opt/local/lib/libsndfile.a
else ifeq ($(UNIX_BASED), true)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
else ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += $(shell pkg-config --cflags openal)
	override LDLIBS += -lALURE32-static -lopenal.dll -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp -lwinmm -lole32
endif
