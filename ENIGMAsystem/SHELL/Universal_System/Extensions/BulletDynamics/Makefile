SOURCES += $(wildcard Universal_System/Extensions/BulletDynamics/*.cpp)
override CFLAGS += $(shell pkg-config --cflags bullet)
override CXXFLAGS += $(shell pkg-config --cflags bullet)
LDLIBS += -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath
