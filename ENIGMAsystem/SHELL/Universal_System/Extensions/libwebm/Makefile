SHARED_SRC_DIR := ../../shared
WEBMPATH := Universal_System/Extensions/libwebm/src

ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), Darwin)
		override CXXFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override CFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override LDFLAGS += -lpthread -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a /opt/local/lib/libvpx.a /opt/local/lib/libz.a /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a -Wno-narrowing -fexceptions
		override LDLIBS += -lpthread  -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a /opt/local/lib/libvpx.a /opt/local/lib/libz.a /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a -Wno-narrowing -fexceptions
	else ifeq ($(OS), FreeBSD)
		override CXXFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override CFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override LDFLAGS += -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a -Wno-narrowing -fexceptions
		override LDLIBS += -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a -Wno-narrowing -fexceptions
	else ifeq ($(OS), DragonFly)
		override CXXFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override CFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override LDFLAGS += -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a -Wno-narrowing -fexceptions
		override LDLIBS += -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a -Wno-narrowing -fexceptions
	else ifeq ($(OS), OpenBSD)
		override CXXFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override CFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override LDFLAGS += -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a -Wno-narrowing -fexceptions
		override LDLIBS += -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a -Wno-narrowing -fexceptions
	else
		override CXXFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override CFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -I$(SHARED_SRC_DIR)/libwebm -Wno-narrowing -fexceptions
		override LDFLAGS += -lpthread -lvorbis -logg -lvpx -lz `pkg-config --libs sdl2 --static` -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a -Wno-narrowing -fexceptions
		override LDLIBS += -lpthread -lvorbis -logg -lvpx -lz `pkg-config --libs sdl2 --static` -lwebm -L$(SHARED_SRC_DIR)/libwebm $(SHARED_SRC_DIR)/libwebm/libwebm.a /usr/local/lib/libvpx.a -Wno-narrowing -fexceptions
	endif
	ifeq ($(OS), Linux)
		override LDFLAGS += -latomic
		override LDLIBS += -latomic
	endif
else
	override CXXFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -Wno-narrowing -fexceptions
	override CFLAGS += -std=c++17 -IUniversal_System/Extensions/libwebm/include/ `pkg-config --cflags sdl2` -Wno-narrowing -fexceptions
	override LDFLAGS += -lpthread -lvorbis -logg -lvpx -lz `pkg-config --libs sdl2 --static` -lwebm -Wno-narrowing -fexceptions
	override LDLIBS += -lpthread -lvorbis -logg -lvpx -lz `pkg-config --libs sdl2 --static` -lwebm -Wno-narrowing -fexceptions
endif

SOURCES += $(WEBMPATH)/audio_decoder.cpp $(WEBMPATH)/frame_buffer.cpp $(WEBMPATH)/timer.cpp $(WEBMPATH)/file_reader.cpp $(WEBMPATH)/libwebmplayer.cpp $(WEBMPATH)/packet.cpp $(WEBMPATH)/utils.cpp $(WEBMPATH)/frame.cpp $(WEBMPATH)/player.cpp $(WEBMPATH)/video_player.cpp $(WEBMPATH)/webm_player.cpp $(WEBMPATH)/yuv_rgb.cpp
