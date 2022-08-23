SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq  ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lALURE32-static -lopenal.dll -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lssp -lwinmm -lole32 -lshlwapi -fstack-protector
else ifeq  ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += /usr/local/lib/libalure-static.a -framework OpenAL /usr/local/lib/libvorbisfile.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libvorbis.a /usr/local/lib/libopusfile.a /usr/local/lib/libopus.a /usr/local/lib/libogg.a /usr/local/lib/libFLAC.a /usr/local/lib/libdumb.a /usr/local/lib/libmodplug.a /usr/local/lib/libmpg123.a
else ifeq  ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth
else ifeq  ($(TARGET-PLATFORM), FreeBSD)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
else ifeq  ($(TARGET-PLATFORM), DragonFlyBSD)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
else ifeq  ($(TARGET-PLATFORM), OpenBSD)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
endif
