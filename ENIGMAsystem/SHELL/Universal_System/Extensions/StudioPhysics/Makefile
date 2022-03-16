SOURCES += $(wildcard Universal_System/Extensions/StudioPhysics/*.cpp) 
ifeq ($(OS), Darwin)
override LDFLAGS += /opt/local/lib/libbox2d.a
override LDLIBS += /opt/local/lib/libbox2d.a
else
override LDFLAGS += -lBox2D
override LDLIBS += -lBox2D
endif
