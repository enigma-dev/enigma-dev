SOURCES += Universal_System/Extensions/GTest/GTest.cpp

override LDLIBS += -lgtest

ifeq ($(TARGET-PLATFORM), Android)
  override CXXFLAGS += -I$(ENIGMA_ROOT)/android/external/gtest/googletest/include
  override ANDROID_MAKEFILES += GTest.mk
	override ANDROID_SHARED_LIBRARIES += gtest
endif
