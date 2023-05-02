SOURCES += Widget_Systems/SDL/dialogs.cpp
IMGUIFD := $(shell Widget_Systems/SDL/imgui.sh)
ifeq ($(UNIX_BASED), false)
	override CFLAGS += -DIFD_SHARED_LIBRARY
	override CXXFLAGS += -DIFD_SHARED_LIBRARY
	override LDFLAGS += "Widget_Systems/SDL/stigma.a"
	override LDLIBS += "Widget_Systems/SDL/stigma.a"
endif
