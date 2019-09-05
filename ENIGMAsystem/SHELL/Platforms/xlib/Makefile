SOURCES += $(wildcard Platforms/xlib/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += -I$(SHARED_SRC_DIR)/libpng-util -I/usr/X11R6/include -I/usr/include -I/usr/include/libpng16
override CFLAGS += -I$(SHARED_SRC_DIR)/libpng-util -I/usr/X11R6/include -I/usr/include -I/usr/include/libpng16
override LDLIBS += -L$(SHARED_SRC_DIR)/libpng-util -Wl,-Bstatic -lpng16 -lz -Wl,-Bdynamic -lz -lpthread -lX11
override LDLIBS += -L/usr/X11R6/lib -L/usr/lib
