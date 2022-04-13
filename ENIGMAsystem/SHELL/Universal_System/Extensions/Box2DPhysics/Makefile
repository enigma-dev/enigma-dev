SOURCES += $(wildcard Universal_System/Extensions/Box2D/*.cpp) 
ifeq ($(OS), Darwin)
	override LDFLAGS += /usr/local/lib/libbox2d.a
	override LDLIBS += /usr/local/lib/libbox2d.a
else
	override LDFLAGS += -lBox2D
	override LDLIBS += -lBox2D
endif
