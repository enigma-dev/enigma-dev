SOURCES += Universal_System/Extensions/Asynchronous/ASYNCdialog.cpp

ifeq ($(PLATFORM), SDL)
	SOURCES += Universal_System/Extensions/Asynchronous/SDLcreateThread.cpp
else ifeq ($(PLATFORM), Win32)
	SOURCES += Universal_System/Extensions/Asynchronous/WIN32createThread.cpp
else
	SOURCES += Universal_System/Extensions/Asynchronous/POSIXcreateThread.cpp
endif
