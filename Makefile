PATH := $(eTCpath)$(PATH)

.PHONY: ENIGMA all clean Game clean-game liblodepng libProtocols libEGM required-directories .FORCE

ENIGMA: .FORCE libProtocols
	$(MAKE) -C CompilerSource

clean: .FORCE
	$(MAKE) -C CompilerSource/ clean
	$(MAKE) -C CommandLine/emake/ clean
	$(MAKE) -C CommandLine/libEGM/ clean
	$(MAKE) -C CommandLine/testing/ clean
	$(MAKE) -C shared/lodepng/ clean
	$(MAKE) -C shared/protos/ clean
	rm -f ./gm2egm

all: liblodepng libProtocols libEGM ENIGMA emake test-runner .FORCE

Game: liblodepng .FORCE
	$(MAKE) -C ENIGMAsystem/SHELL

clean-game: .FORCE
	$(MAKE) -C ENIGMAsystem/SHELL clean

liblodepng: .FORCE
	$(MAKE) -C shared/lodepng/

libProtocols: .FORCE
	$(MAKE) -C shared/protos/

libEGM: .FORCE liblodepng libProtocols
	$(MAKE) -C CommandLine/libEGM/

EMAKE_TARGETS = .FORCE liblodepng

ifneq ($(CLI_ENABLE_EGM), FALSE)
	EMAKE_TARGETS += libEGM
else
	EMAKE_TARGETS += libProtocols
endif

emake: $(EMAKE_TARGETS)
	$(MAKE) -C CommandLine/emake/

gm2egm: libEGM .FORCE
	$(CXX) -Ishared/protos/ -Ishared/protos/codegen -ICommandLine/libEGM/ CommandLine/gm2egm/main.cpp -Wl,-rpath=. -L. -lEGM -lProtocols -o gm2egm

test-runner: emake .FORCE
	$(MAKE) -C CommandLine/testing/

required-directories: .FORCE
	mkdir -p "$(WORKDIR)"
	mkdir -p "$(CODEGEN)Preprocessor_Environment_Editable/"

.FORCE:
