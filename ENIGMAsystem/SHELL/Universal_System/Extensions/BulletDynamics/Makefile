SOURCES += $(wildcard Universal_System/Extensions/BulletDynamics/*.cpp)

ifeq ($(PLATFORM), xlib)
	override CFLAGS += $(shell pkg-config --cflags bullet)
	override CXXFLAGS += $(shell pkg-config --cflags bullet)
	LDLIBS += $(shell pkg-config --libs bullet)
else
	LDLIBS += -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath
endif
