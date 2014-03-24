SOURCES += $(wildcard Networking_Systems/BerkeleySockets/*.cpp)
ifeq ($(PLATFORM), Win32)
	LDLIBS += -lws2_32
else
	LDLIBS += -lcurl 
endif
