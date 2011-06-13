# This is the default makefile. It's included when nothing more specific
# can be found. Specialized makefiles can include it for a generic sweep
# of systems to be compiled with normal settings.
# The default target is `game'.

needout:
ifndef OUTPUTNAME
# @ hides command echo
	@echo No OUTPUTNAME specified. Stopping.
	@exit 2
endif

game: needout build link

build:
	cd Graphics_Systems/$(GRAPHICS)/   && $(MAKE) this MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)" OBJECTS="$(OBJECTS)"
	cd Platforms/$(PLATFORM)/          && $(MAKE) this MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)" OBJECTS="$(OBJECTS)"
	cd Universal_System/               && $(MAKE) this MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)" OBJECTS="$(OBJECTS)"
	cd Audio_Systems/$(AUDIO)/         && $(MAKE) this MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)" OBJECTS="$(OBJECTS)"
	cd Collision_Systems/$(COLLISION)/ && $(MAKE) this MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)" OBJECTS="$(OBJECTS)"
	cd Widget_Systems/$(WIDGETS)/      && $(MAKE) this MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)" OBJECTS="$(OBJECTS)"

link: needout
	g++ SHELLmain.cpp libEGMstd.cpp Graphics_Systems/$(GRAPHICS)/$(OBJECTS)/*.o Platforms/$(PLATFORM)/$(OBJECTS)/*.o Universal_System/$(OBJECTS)/*.o Audio_Systems/$(AUDIO)/$(OBJECTS)/*.o Collision_Systems/$(COLLISION)/$(OBJECTS)/*.o Audio_Systems/$(AUDIO)/$(OBJECTS)/*.a Audio_Systems/$(AUDIO)/$(OBJECTS)/libogg.a Widget_Systems/$(WIDGETS)/$(OBJECTS)/*.o $(GLINKS) -o "$(OUTPUTNAME)"
	@echo Built to "$(OUTPUTNAME)"

clean:
	@echo $(OS)
	cd Graphics_Systems/OpenGL/   && $(MAKE) clean
	cd Graphics_Systems/OpenGLES/ && $(MAKE) clean
	cd Audio_Systems/OpenAL/      && $(MAKE) clean
	cd Platforms/Win32/           && $(MAKE) clean
	cd Platforms/xlib/            && $(MAKE) clean
	cd Universal_System/          && $(MAKE) clean
	cd Collision_Systems/None     && $(MAKE) clean

