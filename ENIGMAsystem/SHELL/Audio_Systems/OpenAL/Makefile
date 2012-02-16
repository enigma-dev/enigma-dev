SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)

ifeq ($(PLATFORM), xlib)
	LDLIBS += -lopenal -lalure -lvorbisfile -lvorbis -ldumb
else ifeq ($(PLATFORM), Win32)
	LDLIBS += -lopenal -lalure -lvorbisfile -lvorbis -ldumb
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenAL
endif
