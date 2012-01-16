SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp) $(wildcard Audio_Systems/OpenAL/alure/*.cpp)

ifeq ($(PLATFORM), xlib)
	LDLIBS += -lopenal -lvorbisfile -lvorbis -ldumb
else ifeq ($(PLATFORM), Win32)
	include $(addsuffix /Makefile, $(addprefix Audio_Systems/OpenAL/, alure Codecs)) ## Fix Me: All this shit was moved.
	LDLIBS += additional/al/lib/Win32/OpenAL32.lib
	RESOURCES += additional/al/rc/oal32.rc
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenAL
endif
