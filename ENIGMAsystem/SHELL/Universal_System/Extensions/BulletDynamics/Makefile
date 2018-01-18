SOURCES += $(wildcard Universal_System/Extensions/BulletDynamics/*.cpp)

ifeq ($(PLATFORM), xlib)
	override CFLAGS += $(shell pkg-config --cflags bullet)
	override CXXFLAGS += $(shell pkg-config --cflags bullet)
	override LDLIBS += $(shell pkg-config --libs bullet)
else
	override LDLIBS += -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath
endif
