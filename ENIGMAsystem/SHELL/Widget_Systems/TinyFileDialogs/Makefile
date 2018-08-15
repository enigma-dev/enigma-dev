include $(SHARED_SRC_DIR)/tinyfiledialogs/Makefile

ifeq ($(TARGET-PLATFORM), MacOSX)
	SOURCES += $(wildcard Widget_Systems/TinyFileDialogs/dialogs-macos.cpp)

ifeq ($(TARGET-PLATFORM), Linux)
	SOURCES += $(wildcard Widget_Systems/TinyFileDialogs/dialogs-linux.cpp)
