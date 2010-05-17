ENIGMA:
	cd CompilerSource && $(MAKE) $(PLATFORM)

# Target takes the following parameters:
#	GMODE{run,build,debug,compile}
#	GRAPHICS{<anything under graphics_systems>}
#	PLATFORM{<anything under platforms>}
#	GLINKS{<requirements of anything above>}
Game:
	echo Okay.
	cd ENIGMAsystem/SHELL/ && $(MAKE) GMODE=$(GMODE) GLINKS="$(GLINKS)" GRAPHICS=$(GRAPHICS) PLATFORM=$(PLATFORM)

