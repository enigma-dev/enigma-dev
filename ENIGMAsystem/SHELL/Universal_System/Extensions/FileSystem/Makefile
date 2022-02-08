ifeq ($(OS), Linux)
	override LDLIBS += -lpthread
endif
ifeq ($(OS), FreeBSD)
	override LDLIBS += -lc -lpthread
endif
ifeq ($(OS), DragonFly)
	override LDLIBS += -lc -lpthread
endif
ifeq ($(OS), OpenBSD)
	override LDLIBS += -lc -lpthread
endif
SOURCES += Universal_System/Extensions/FileSystem/filesystem.cpp
SOURCES += Universal_System/Extensions/FileSystem/enigma.cpp
