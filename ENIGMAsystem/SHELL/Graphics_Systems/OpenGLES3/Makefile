include Graphics_Systems/OpenGLES/Makefile

override CXXFLAGS += -I$(ENIGMA_ROOT)/android/external/gles3/include
override CFLAGS += -I$(ENIGMA_ROOT)/android/external/gles3/include 
SOURCES += $(wildcard Graphics_Systems/OpenGLES3/*.cpp) $(ENIGMA_ROOT)/android/external/gles3/src/gles2.c

ifeq ($(TARGET-PLATFORM), Android)
	ANDROID_LDLIBS += -lGLESv3
endif
