# This is the default makefile. It's included when nothing more specific can be found.
# The default target is `game'.

needout:
ifndef OUTPUTNAME
# @ hides command echo
	@echo No OUTPUTNAME specified. Stopping.
	@exit 2
endif

game: needout build link

build:
	cd Graphics_Systems/$(GRAPHICS)/   && $(MAKE) $(GMODE) MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd Platforms/$(PLATFORM)/          && $(MAKE) $(GMODE) MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd Universal_System/               && $(MAKE) $(GMODE) MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd Audio_Systems/$(AUDIO)/         && $(MAKE) $(GMODE) MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd Collision_Systems/$(COLLISION)/ && $(MAKE) $(GMODE) MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"
	cd Widget_Systems/$(WIDGETS)/      && $(MAKE) $(GMODE) MODE=$(GMODE) GFLAGS="$(GFLAGS)" ECFLAGS="$(ECFLAGS)" ECPPFLAGS="$(ECPPFLAGS)"

link: needout
	g++ SHELLmain.cpp libEGMstd.cpp Graphics_Systems/$(GRAPHICS)/.eobjs_$(GMODE)/*.o Platforms/$(PLATFORM)/.eobjs_$(GMODE)/*.o Universal_System/.eobjs_$(GMODE)/*.o Audio_Systems/$(AUDIO)/.eobjs_$(GMODE)/*.o Collision_Systems/$(COLLISION)/.eobjs_$(GMODE)/*.o Audio_Systems/$(AUDIO)/.eobjs_$(GMODE)/*.a Audio_Systems/$(AUDIO)/.eobjs_$(GMODE)/libogg.a Widget_Systems/$(WIDGETS)/.eobjs_$(GMODE)/*.o $(GLINKS) -o $(OUTPUTNAME)
	@echo Built to "$(OUTPUTNAME)"

clean:
	@echo $(OS)
	cd Graphics_Systems/OpenGL/   && $(MAKE) clean
	cd Graphics_Systems/OpenGLES/ && $(MAKE) clean
	cd Audio_Systems/OpenAL/      && $(MAKE) clean
	cd Platforms/Win32/           && $(MAKE) clean
	cd Platforms/xlib/            && $(MAKE) clean
	cd Universal_System/          && $(MAKE) clean
	cd Collision_Systems/none     && $(MAKE) clean

