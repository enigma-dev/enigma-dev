PATH := $(eTCpath)$(PATH)

.PHONY: ENIGMA all clean Game clean-game .FORCE

ENIGMA: .FORCE libProtocols liblodepng
	$(MAKE) -C CompilerSource 

clean:
	$(MAKE) -C CompilerSource/ clean
	$(MAKE) -C CommandLine/emake/ clean
	$(MAKE) -C CommandLine/libGMX/ clean
	$(MAKE) -C CommandLine/protos/ clean
	$(MAKE) -C CommandLine/testing/ clean
	$(MAKE) -C lodepng/ clean
	
all:
	$(MAKE) -C lodepng/
	$(MAKE) -C CommandLine/protos/
	$(MAKE) -C CommandLine/libGMX/
	$(MAKE) -C CompilerSource/
	$(MAKE) -C CommandLine/emake/
	$(MAKE) -C CommandLine/testing/

Game: liblodepng
	$(MAKE) -C ENIGMAsystem/SHELL

clean-game:
	$(MAKE) -C ENIGMAsystem/SHELL clean
	
liblodepng:
	$(MAKE) -C lodepng/

libProtocols: .FORCE
	$(MAKE) -C CommandLine/protos/

libGMX: .FORCE libProtocols
	$(MAKE) -C CommandLine/libGMX/

emake: libGMX .FORCE
	$(MAKE) -C CommandLine/emake/

test-runner: emake .FORCE
	$(MAKE) -C CommandLine/testing/

required-directories:
	mkdir -p "$(WORKDIR)"
	mkdir -p "$(CODEGEN)Preprocessor_Environment_Editable/"

.FORCE:
