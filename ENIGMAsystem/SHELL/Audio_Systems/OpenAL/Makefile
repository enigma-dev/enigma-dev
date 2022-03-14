SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenAL -lalure
else ifeq ($(UNIX_BASED), true)
	override LDLIBS += -lopenal -lalure -lvorbisfile -lvorbis -ldumb
else ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += $(shell pkg-config --cflags openal)
	override LDLIBS += -lALURE32-static -lopenal.dll -lvorbisfile -lvorbis -logg -lFLAC -lsndfile -lmodplug -lmpg123 -lole32 -lwinmm
endif
