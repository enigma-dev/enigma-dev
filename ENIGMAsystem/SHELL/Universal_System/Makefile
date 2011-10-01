SOURCES += $(wildcard Universal_System/*.cpp)

ifeq ($(PLATFORM), Win32)
	LDLIBS += ../additional/zlib/libzlib.a
else
	LDLIBS += -lz
endif
