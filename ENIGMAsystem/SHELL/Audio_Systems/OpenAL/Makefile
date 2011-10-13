SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)
include $(addsuffix /Makefile, $(addprefix Audio_Systems/OpenAL/, alure ../Codecs))

ifeq ($(PLATFORM), xlib)
	LDLIBS += -lopenal
else ifeq ($(PLATFORM), Win32)
	LDLIBS += ../additional/al/lib/Win32/OpenAL32.lib
	RESOURCES += ../additional/al/rc/oal32.rc
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenAL
endif
