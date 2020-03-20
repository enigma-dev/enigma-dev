OS := $(shell uname -s)
ifeq ($(OS), Linux)
	UNIX_BASED := true
else ifeq ($(OS), FreeBSD)
	UNIX_BASED := true
else ifeq ($(OS), Darwin)
	UNIX_BASED := true
else 
	UNIX_BASED := false
endif

