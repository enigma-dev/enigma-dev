SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(PLATFORM), xlib)
	LDLIBS += -lopenal -lalure -lvorbisfile -lvorbis -ldumb
else ifeq ($(PLATFORM), Win32)
	include ../Additional/Windows/alure/Makefile
	LDLIBS += -lOpenAL32 -lalure32 -lvorbisfile -lvorbis -ldumb -logg
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenAL
	SOURCES += $(wildcard ../Additional/alure/enigma_alure/*.cpp)
	include $(addsuffix /Makefile, ../additional/alure/Codecs)
endif
