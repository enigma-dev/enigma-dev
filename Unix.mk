OS := $(shell uname -s)
ifeq ($(OS), Linux)
	UNIX_BASED := true
	UNIX_MACOS := false
else ifeq ($(OS), FreeBSD)
	UNIX_BASED := true
	UNIX_MACOS := false
else ifeq ($(OS), Darwin)
	UNIX_BASED := true
	UNIX_MACOS := true
else 
	UNIX_BASED := false
	UNIX_MACOS := false
endif

