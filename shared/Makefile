# This is the "shared" makefile. It defines routines for use in other makefiles.
# This directory does not have one makefile to rule them all.
# Refer to library directories within this dir for their respective makefiles.

arbitrary_relative_wildcard=$(patsubst $1/%,%,$(wildcard $1/$2))
relative_wildcard=$(call arbitrary_relative_wildcard,$(dir $(lastword $(MAKEFILE_LIST))),$1)
