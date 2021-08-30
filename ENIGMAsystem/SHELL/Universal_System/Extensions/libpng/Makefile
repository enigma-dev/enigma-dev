SHARED_SRC_DIR := ../../shared
override CXXFLAGS += -I$(SHARED_SRC_DIR)/libpng-util
override LDLIBS += -lpng -lz

SOURCES += Universal_System/Extensions/libpng/libpng-ext.cpp Universal_System/Extensions/libpng/libpng-util.cpp

ifeq ($(TARGET-PLATFORM), Android)
	override CXXFLAGS += -I$(ENIGMA_ROOT)/android/external/libpng
  override ANDROID_MAKEFILES += libpng.mk
	override ANDROID_SHARED_LIBRARIES += libpng
endif
