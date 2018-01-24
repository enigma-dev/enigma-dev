PATH := $(eTCpath)$(PATH)

.PHONY: ENIGMA clean Game clean-game codegen .FORCE

ENIGMA:
	$(MAKE) -C CompilerSource 

clean:
	$(MAKE) -C CompilerSource clean

Game:
	$(MAKE) -C ENIGMAsystem/SHELL

clean-game:
	$(MAKE) -C ENIGMAsystem/SHELL clean

emake: .FORCE
	$(MAKE) -C CommandLine/emake/

test-runner: emake .FORCE
	$(MAKE) -C CommandLine/testing/

required-directories:
	mkdir -p "$(WORKDIR)"
	mkdir -p "$(CODEGEN)Preprocessor_Environment_Editable/"

.FORCE:
