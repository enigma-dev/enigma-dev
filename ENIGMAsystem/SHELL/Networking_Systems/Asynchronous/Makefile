SOURCES += $(wildcard Networking_Systems/Asynchronous/*.cpp)
ifeq ($(PLATFORM), Win32)
	override LDLIBS += -lws2_32
else
	override LDLIBS += -lcurl 
endif
