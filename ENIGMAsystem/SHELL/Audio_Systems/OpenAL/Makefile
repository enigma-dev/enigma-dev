SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq  ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += $(shell pkg-config --cflags openal)
	override CXXFLAGS += $(shell pkg-config --cflags openal)
	override LDLIBS += -lALURE32-static -lopenal.dll -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lssp -lwinmm -lole32 -lshlwapi -fstack-protector
else ifeq  ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth
else ifeq  ($(TARGET-PLATFORM), FreeBSD)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
else ifeq  ($(TARGET-PLATFORM), DragonFlyBSD)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
else ifeq  ($(TARGET-PLATFORM), OpenBSD)
	override CXXFLAGS += -I../../shared/alure/include
	override LDLIBS += ../../shared/alure/libalure-static.a -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth
endif
