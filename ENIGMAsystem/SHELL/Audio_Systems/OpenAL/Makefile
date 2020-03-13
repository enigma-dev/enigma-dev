SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(filter ($(TARGET-PLATFORM), Linux), ($(TARGET-PLATFORM), FreeBSD)))
	override LDLIBS += -lopenal -lalure -lvorbisfile -lvorbis -ldumb
else ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += $(shell pkg-config --cflags openal)
	override LDLIBS += -lALURE32-static -lopenal.dll -lvorbisfile -lvorbis -logg -lFLAC -lsndfile -lmodplug -lmpg123 -lole32 -lwinmm
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenAL
	SOURCES += $(wildcard ../additional/alure/enigma_alure/*.cpp)
	include $(addsuffix /Makefile, ../additional/alure/Codecs)
endif
