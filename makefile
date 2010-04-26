ENIGMA:
	cd CompilerSource && $(MAKE) Release
Game:
	# Target takes the following parameters:
	#	GMODE{run,build,debug,compile}
	#	GRAPHICS{<anything under graphics_systems>}
	#	PLATFORM{<anything under platforms>}
	#	GLINKS{<requirements of anything above>}
	echo Okay.
	cd ENIGMAsystem/SHELL/ && make GMODE=$(GMODE) GLINKS="$(GLINKS)" GRAPHICS=$(GRAPHICS) PLATFORM=$(PLATFORM)

