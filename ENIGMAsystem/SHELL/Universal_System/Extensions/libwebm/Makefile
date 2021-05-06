override CXXFLAGS += -I$(SHARED_SRC_DIR)/libwebm -IUniversal_System/Extensions/libwebm/include -fexceptions
override CFLAGS += -I$(SHARED_SRC_DIR)/libwebm -IUniversal_System/Extensions/libwebm/include -fexceptions
SOURCES += $(wildcard Universal_System/Extensions/libwebm/src/*.cpp) $(SHARED_SRC_DIR)libwebm/libwebm.a
override LDFLAGS += -lvorbis -logg -lvpx -lz -lc `pkg-config --cflags --libs --static sdl2` -lpthread -lc -L../../shared/libwebm/ -fexceptions -Wno-narrowing -fPIC
