SHARED_SRC_DIR := ../../shared
WEBMPATH := Universal_System/Extensions/libwebm/src

ifeq ($(UNIX_BASED), true)
	override CXXFLAGS +=  -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
	override CFLAGS += -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
	override LDFLAGS += -lpthread -lvorbis -logg -lvpx -lz `pkg-config --libs sdl2 --static` -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a -Wno-narrowing -fexceptions
	override LDLIBS += -lpthread -lvorbis -logg -lvpx -lz `pkg-config --libs sdl2 --static` -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a -Wno-narrowing -fexceptions
	ifeq ($(OS), Linux)
		override LDFLAGS += -latomic
		override LDLIBS += -latomic
	else ifeq ($(OS), FreeBSD)
		override LDFLAGS += -lc
		override LDLIBS += -lc
	else ifeq ($(OS), DragonFly)
		override LDFLAGS += -lc
		override LDLIBS += -lc
	endif
	SOURCES += $(WEBMPATH)/yuv_rgb.cpp
else
	override CXXFLAGS += -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -Wno-narrowing -fexceptions
	override CFLAGS += -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -Wno-narrowing -fexceptions
	override LDFLAGS += -lpthread -lvorbis -logg -lvpx -lz `pkg-config --libs sdl2 --static` -lwebm -Wno-narrowing -fexceptions
	override LDLIBS += -lpthread -lvorbis -logg -lvpx -lz `pkg-config --libs sdl2 --static` -lwebm -Wno-narrowing -fexceptions
endif

SOURCES += $(WEBMPATH)/audio_decoder.cpp $(WEBMPATH)/frame_buffer.cpp $(WEBMPATH)/timer.cpp $(WEBMPATH)/file_reader.cpp $(WEBMPATH)/libwebmplayer.cpp $(WEBMPATH)/packet.cpp $(WEBMPATH)/utils.cpp $(WEBMPATH)/frame.cpp $(WEBMPATH)/player.cpp $(WEBMPATH)/video_player.cpp $(WEBMPATH)/webm_player.cpp
