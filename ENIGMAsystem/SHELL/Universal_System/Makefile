SOURCES += $(wildcard Universal_System/*.cpp)

ifeq ($(PLATFORM), Win32)
	LDLIBS += additional/zlib/libzlib.a ## Fix Me: additional is missing
else
	LDLIBS += -lz
endif
