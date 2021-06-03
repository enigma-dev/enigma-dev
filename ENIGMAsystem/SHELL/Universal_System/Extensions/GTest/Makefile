SOURCES += Universal_System/Extensions/GTest/GTest.cpp

ifeq ($(TARGET_PLATFORM), Windows)
	override LDLIBS += -lgtest.dll
else
	override LDLIBS += -lgtest
endif
