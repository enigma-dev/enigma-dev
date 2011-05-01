game:
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/ && ant debug
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/bin/ && /Users/alasdairmorrison/Documents/AndroidSDK/tools/adb devices
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/bin/ && /Users/alasdairmorrison/Documents/AndroidSDK/tools/adb install -r EnigmaAndroidGame-debug.apk
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/bin/ && /Users/alasdairmorrison/Documents/AndroidSDK/tools/emulator -avd my_avd
	cd ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/bin/ && /Users/alasdairmorrison/Documents/AndroidSDK/tools/adb wait-for-device shell am start -a android.intent.action.MAIN -n org.enigmadev/.EnigmaActivity
	
