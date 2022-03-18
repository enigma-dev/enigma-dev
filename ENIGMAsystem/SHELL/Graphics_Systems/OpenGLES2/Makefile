include Graphics_Systems/OpenGLES/Makefile
override CXXFLAGS += -I$(ENIGMA_ROOT)/android/external/gles2/include -IGraphics_Systems/OpenGLES2/wrappers/
override CFLAGS += -I$(ENIGMA_ROOT)/android/external/gles2/include
SOURCES += $(wildcard Graphics_Systems/OpenGLES2/*.cpp) $(wildcard Graphics_Systems/OpenGLES2/wrappers/*.cpp) $(ENIGMA_ROOT)/android/external/gles2/src/gles2.c

ifeq ($(TARGET-PLATFORM), Android)
	ANDROID_LDLIBS += -lGLESv2
endif
