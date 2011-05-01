ifdef eTCpath
	PATH := $(eTCpath)$(PATH)
endif

ENIGMA: DUMMYDEPENDENCY
	cd CompilerSource && $(MAKE)
DUMMYDEPENDENCY:

# Target takes the following parameters:
#	GMODE{run,build,debug,compile}
#	GRAPHICS{<anything under graphics_systems>}
#	PLATFORM{<anything under platforms>}
#	GLINKS{<requirements of anything above>}
Game:
	cd ENIGMAsystem/SHELL/ && $(MAKE) GMODE=$(GMODE) GLINKS="$(GLINKS)" GFLAGS="$(GFLAGS)" GRAPHICS=$(GRAPHICS) PLATFORM=$(PLATFORM) COMPILEPATH=$(COMPILEPATH) OUTPUTNAME="$(OUTPUTNAME)"

clean:
	cd CompilerSource && $(MAKE) clean

clean-game:
	cd ENIGMAsystem/SHELL/ && $(MAKE) clean


