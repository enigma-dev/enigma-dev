include Graphics_Systems/OpenGLES/Makefile

ifeq ($(TARGET-PLATFORM), Android)
	override CXXFLAGS += -I$(ENIGMA_ROOT)/android/external/gles2/include
	override CFLAGS += -I$(ENIGMA_ROOT)/android/external/gles2/include
	SOURCES += $(wildcard Graphics_Systems/OpenGLES2/*.cpp) $(ENIGMA_ROOT)/android/external/gles2/src/gles2.c
	ANDROID_LDLIBS += -lGLESv2
endif
