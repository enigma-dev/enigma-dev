ENIGMA: DUMMYDEPENDENCY
	cd CompilerSource && $(MAKE) $(PLATFORM)
DUMMYDEPENDENCY:
	
# Target takes the following parameters:
#	GMODE{run,build,debug,compile}
#	GRAPHICS{<anything under graphics_systems>}
#	PLATFORM{<anything under platforms>}
#	GLINKS{<requirements of anything above>}
Game:
	echo Okay.
	cd ENIGMAsystem/SHELL/ && $(MAKE) GMODE=$(GMODE) GLINKS="$(GLINKS)" GFLAGS="$(GFLAGS)" GRAPHICS=$(GRAPHICS) PLATFORM=$(PLATFORM)

clean:
	cd CompilerSource && $(MAKE) clean
