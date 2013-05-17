SOURCES += $(wildcard Audio_Systems/SFML/*.cpp)

ifeq ($(PLATFORM), Win32)
	LDLIBS += -lsfml-audio-s -lsfml-system-s -lOpenAL32-static -lsndfile -ldsound -lwinmm -lole32
else
	LDLIBS += -lsfml-audio -lsfml-system
endif
