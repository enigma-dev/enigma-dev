PATH := $(eTCpath)$(PATH)

.PHONY: ENIGMA all clean Game clean-game liblodepng libProtocols libEGM required-directories .FORCE

ENIGMA: .FORCE libProtocols liblodepng
	$(MAKE) -C CompilerSource

clean: .FORCE
	$(MAKE) -C CompilerSource/ clean
	$(MAKE) -C CommandLine/emake/ clean
	$(MAKE) -C CommandLine/libEGM/ clean
	$(MAKE) -C CommandLine/protos/ clean
	$(MAKE) -C CommandLine/testing/ clean
	$(MAKE) -C lodepng/ clean

all: liblodepng libProtocols libEGM ENIGMA emake test-runner .FORCE

Game: liblodepng .FORCE
	$(MAKE) -C ENIGMAsystem/SHELL

clean-game:
	$(MAKE) -C ENIGMAsystem/SHELL clean

liblodepng: .FORCE
	$(MAKE) -C shared/lodepng/

libProtocols: .FORCE
	$(MAKE) -C CommandLine/protos/

libEGM: .FORCE libProtocols
	$(MAKE) -C CommandLine/libEGM/

emake: ENIGMA .FORCE
	$(MAKE) -C CommandLine/emake/

test-runner: emake .FORCE
	$(MAKE) -C CommandLine/testing/

required-directories: .FORCE
	mkdir -p "$(WORKDIR)"
	mkdir -p "$(CODEGEN)Preprocessor_Environment_Editable/"

.FORCE:
