SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(PLATFORM), xlib)
	LDLIBS += -lopenal -lalure -lvorbisfile -lvorbis -ldumb
else ifeq ($(PLATFORM), Win32)
	LDLIBS += -lopenal32 -lalure32 -lvorbisfile -lvorbis -ldumb
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenAL
	SOURCES += $(wildcard ../additional/alure/enigma_alure/*.cpp)
	include $(addsuffix /Makefile, ../additional/alure/Codecs)
endif
