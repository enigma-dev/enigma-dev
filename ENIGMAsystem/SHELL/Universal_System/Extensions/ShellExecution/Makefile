SHARED_SRC_DIR := ../../shared
override CXXFLAGS += -I$(SHARED_SRC_DIR)/apiprocess -std=c++17
override CFLAGS += -I$(SHARED_SRC_DIR)/apiprocess -std=c++17
ifeq ($(OS), Linux)
	override LDLIBS += -lprocps
	override LDFLAGS += -lprocps
else ifeq ($(OS), FreeBSD)
	override LDLIBS += -lc /usr/lib/libelf.a  /usr/lib/libkvm.a /usr/lib/libutil.a /usr/lib/libprocstat.a
	override LDFLAGS += -lc /usr/lib/libelf.a  /usr/lib/libkvm.a /usr/lib/libutil.a /usr/lib/libprocstat.a
else ifeq ($(OS), DragonFly)
	override LDLIBS += -lc /usr/lib/libelf.a  /usr/lib/libkvm.a
	override LDFLAGS += -lc /usr/lib/libelf.a  /usr/lib/libkvm.a
else ifeq ($(OS), OpenBSD)
	override LDLIBS += -lc /usr/lib/libelf.a  /usr/lib/libkvm.a
	override LDFLAGS += -lc /usr/lib/libelf.a  /usr/lib/libkvm.a
endif
SOURCES += Universal_System/Extensions/ShellExecution/shell.cpp