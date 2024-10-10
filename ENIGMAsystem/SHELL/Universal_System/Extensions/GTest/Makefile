SOURCES += Universal_System/Extensions/GTest/GTest.cpp

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lgtest.dll
else
	override LDLIBS += -lgtest
endif

ifeq ($(TARGET-PLATFORM), Android)
  override CXXFLAGS += -I$(ENIGMA_ROOT)/android/external/gtest/googletest/include
  override ANDROID_MAKEFILES += GTest.mk
	override ANDROID_SHARED_LIBRARIES += gtest
endif
