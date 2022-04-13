override CXXFLAGS += -IBridges/OpenGL

ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += -DGLEW_STATIC
	override LDLIBS += -lglew32 -lopengl32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL -lGLEW
else ifeq ($(TARGET-PLATFORM), FreeBSD)
	override CFLAGS += -DGLEW_STATIC
	override CXXFLAGS += -DGLEW_STATIC
	override LDLIBS += -lGL /usr/local/lib/libGLEW.a -lGLEW
else ifeq ($(TARGET-PLATFORM), DragonFlyBSD)
	override CFLAGS += -DGLEW_STATIC
	override CXXFLAGS += -DGLEW_STATIC
	override LDLIBS += -lGL /usr/local/lib/libGLEW.a -lGLEW
else ifeq ($(TARGET-PLATFORM), OpenBSD)
	override CFLAGS += -DGLEW_STATIC
	override CXXFLAGS += -DGLEW_STATIC
	override LDLIBS += -lGL /usr/local/lib/libGLEW.a -lGLEW
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL /usr/local/lib/libGLEW.a
endif
