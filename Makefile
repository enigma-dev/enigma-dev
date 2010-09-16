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
	cd ENIGMAsystem/SHELL/ && $(MAKE) GMODE=$(GMODE) GLINKS="$(GLINKS)" GFLAGS="$(GFLAGS)" GRAPHICS=$(GRAPHICS) PLATFORM=$(PLATFORM) OUTPUTNAME="$(OUTPUTNAME)"

clean:
	cd CompilerSource && $(MAKE) clean

clean-game:
	cd ENIGMAsystem/SHELL/ && $(MAKE) clean

iphone:
	cd MacOS/ && xcodebuild -target EnigmaIphone -sdk iphonesimulator3.0

iphonedevice:
	cd MacOS/ && xcodebuild -target EnigmaIphone -sdk iphoneos2.2.1
