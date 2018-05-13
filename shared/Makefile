arbitrary_relative_wildcard=$(patsubst $1/%,%,$(wildcard $1/$2))
relative_wildcard=$(call arbitrary_relative_wildcard,$(dir $(lastword $(MAKEFILE_LIST))),$1)
