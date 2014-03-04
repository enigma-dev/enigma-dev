SOURCES += $(wildcard Networking_Systems/BerkeleySockets/*.cpp)
LDLIBS += -lcurl 
ifeq ($(PLATFORM), Win32)
	LDLIBS += -lws2_32
endif
