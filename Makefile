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
	echo Okay.zzzzz
  ifeq	"$(PLATFORM)" 	"iPhone"

	cd MacOS/ && xcodebuild -target EnigmaIphone -sdk iphonesimulator3.0

 else ifeq	"$(PLATFORM)" 	"Android"

#	$(CREMOVE)ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/obj/local$(SLASHC)*.*$(ENDCREMOVE) #first of all delete objects
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/jni && /Users/alasdairmorrison/Documents/AndroidSDK/crystax/ndk-build

  else
	cd ENIGMAsystem/SHELL/ && $(MAKE) GMODE=$(GMODE) GLINKS="$(GLINKS)" GFLAGS="$(GFLAGS)" GRAPHICS=$(GRAPHICS) PLATFORM=$(PLATFORM) COMPILEPATH=$(COMPILEPATH) OUTPUTNAME="$(OUTPUTNAME)"
  endif
clean:
	cd CompilerSource && $(MAKE) clean

clean-game:
	cd ENIGMAsystem/SHELL/ && $(MAKE) clean

iphone:
	cd MacOS/ && xcodebuild -target EnigmaIphone -sdk iphonesimulator3.0

iphonedevice:
	cd MacOS/ && xcodebuild -target EnigmaIphone -sdk iphoneos2.2.1


android:
	-rm ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/obj/local*/* #first of all delete objects
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/jni && /Users/alasdairmorrison/Documents/AndroidSDK/crystax/ndk-build

androidrun:
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/ && ant debug
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/bin/ && /Users/alasdairmorrison/Documents/AndroidSDK/tools/adb devices
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/bin/ && /Users/alasdairmorrison/Documents/AndroidSDK/tools/adb install -r EnigmaAndroidGame-debug.apk
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/bin/ && /Users/alasdairmorrison/Documents/AndroidSDK/tools/emulator -avd my_avd
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/bin/ && /Users/alasdairmorrison/Documents/AndroidSDK/tools/adb wait-for-device shell am start -a android.intent.action.MAIN -n org.enigmadev/.EnigmaActivity

