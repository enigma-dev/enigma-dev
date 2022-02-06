ifeq ($(OS), FreeBSD)
	override LDLIBS += -lc
endif
ifeq ($(OS), DragonFly)
	override LDLIBS += -lc
endif
ifeq ($(OS), OpenBSD)
	override LDLIBS += -lkvm -lc
endif
SOURCES += Universal_System/Extensions/FileSystem/filesystem.cpp
SOURCES += Universal_System/Extensions/FileSystem/enigma.cpp
