include $(SHARED_SRC_DIR)/tinyfiledialogs/Makefile

ifeq ($(TARGET-PLATFORM), MacOSX)
	SOURCES += Widget_Systems/TinyFileDialogs/macos.cpp
else ifeq ($(TARGET-PLATFORM), Linux)
	SOURCES += Widget_Systems/TinyFileDialogs/linux.cpp
endif

SOURCES += Widget_Systems/TinyFileDialogs/dialogs.cpp
