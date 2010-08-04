# Target `all` takes the following parameters:
#	GMODE{run,build,debug,compile}
#	GRAPHICS{<anything under graphics_systems>}
#	PLATFORM{<anything under platforms>}
#	GLINKS{<anything needed by the above as linker args>}
all:	
	cd Graphics_Systems/$(GRAPHICS)/ 		&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="$(GFLAGS)"
	cd Platforms/$(PLATFORM)/ 			&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="$(GFLAGS)"
	cd Universal_System/				&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="$(GFLAGS)"
	cd Audio_Systems/OpenAL/			&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="$(GFLAGS)"
	
	g++ SHELLmain.cpp libEGMstd.cpp Graphics_Systems/$(GRAPHICS)/.eobjs_$(GMODE)/*.o Platforms/$(PLATFORM)/.eobjs_$(GMODE)/*.o Universal_System/.eobjs_$(GMODE)/*.o Audio_Systems/OpenAL/.eobjs_$(GMODE)/*.o Audio_Systems/OpenAL/.eobjs_$(GMODE)/*.a Audio_Systems/OpenAL/.eobjs_$(GMODE)/libogg.a $(GLINKS) -o game.exe

clean:
	cd Graphics_Systems/OpenGL/ 	&& $(MAKE) clean
	cd Audio_Systems/OpenAL/ 	&& $(MAKE) clean
	cd Platforms/windows/ 		&& $(MAKE) clean
	cd Platforms/xlib/ 		&& $(MAKE) clean
	cd Universal_System/		&& $(MAKE) clean
