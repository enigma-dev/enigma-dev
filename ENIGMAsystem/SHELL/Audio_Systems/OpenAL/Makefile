SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(PLATFORM), xlib)
	LDLIBS += -lopenal -lalure -lvorbisfile -lvorbis -ldumb
else ifeq ($(PLATFORM), Win32)
	LDLIBS += -lALURE32-static -lOpenAL32 -lole32 -lwinmm -lvorbis
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenAL
	SOURCES += $(wildcard ../Additional/alure/enigma_alure/*.cpp)
	include $(addsuffix /Makefile, ../additional/alure/Codecs)
endif
