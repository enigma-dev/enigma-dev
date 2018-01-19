SOURCES += $(wildcard Graphics_Systems/OpenGL3/*.cpp) $(wildcard Graphics_Systems/General/*.cpp) Graphics_Systems/General/glew.c

ifeq ($(PLATFORM), Win32)
	override LDLIBS += -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	override LDLIBS += -lGL -lGLU
else ifeq ($(PLATFORM), Cocoa)
	override LDLIBS += -framework OpenGL
endif

#$(OBJDIR)/Graphics_Systems/OpenGL3/glew.o: Graphics_Systems/OpenGL3/glew.c
#	gcc -c $^ -O3 -DGLEW_STATIC -o $@

