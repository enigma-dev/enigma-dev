PATH := $(eTCpath)$(PATH)

.PHONY: ENIGMA

ENIGMA:
	$(MAKE) -j 3 -C CompilerSource 

clean:
	$(MAKE) -C CompilerSource clean

Game:
	$(MAKE) -C ENIGMAsystem/SHELL

clean-game:
	$(MAKE) -C ENIGMAsystem/SHELL clean


