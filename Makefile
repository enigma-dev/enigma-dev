PATH := $(eTCpath)$(PATH)

.PHONY: ENIGMA clean Game clean-game

ENIGMA:
	$(MAKE) -C CompilerSource 

clean:
	$(MAKE) -C CompilerSource clean

Game:
	$(MAKE) -C ENIGMAsystem/SHELL

clean-game:
	$(MAKE) -C ENIGMAsystem/SHELL clean
	
emake:
	$(MAKE) -C CommandLine/emake/

test-runner:
	$(MAKE) -C CommandLine/testing/


