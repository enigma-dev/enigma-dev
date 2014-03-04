SOURCES += $(wildcard Audio_Systems/SFML/*.cpp)

ifeq ($(PLATFORM), Win32)
	LDLIBS += -lsfml-audio-s -lsfml-system-s -lOpenAL32-static -lsndfile -ldsound -lwinmm -lole32
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework SFML -framework sfml-audio -framework sfml-system -framework OpenAL -framework sndfile
else
  LDLIBS += -lsfml-audio -lsfml-system
endif
