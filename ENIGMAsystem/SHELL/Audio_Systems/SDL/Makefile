ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard Audio_Systems/SDL/*.cpp)

ifeq ($(TARGET-PLATFORM), Android)
  override ANDROID_MAKEFILES += SDL_mixer/Android.mk
  override ANDROID_SHARED_LIBRARIES += SDL2_mixer
endif

override CXXFLAGS += $(shell pkg-config --cflags sdl)
override LDLIBS += $(shell pkg-config --libs sdl2 SDL2_mixer) -lopusfile -lopus -lvorbisfile -lvorbis -logg -lFLAC -lsndfile -lmodplug -lmpg123 -lole32 -lwinmm -lm -lshlwapi -lssp

