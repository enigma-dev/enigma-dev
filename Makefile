include Config.mk

PATH := $(eTCpath)$(PATH)
SHELL=/bin/bash

.PHONY: ENIGMA all clean Game clean-game clean-protos emake emake-tests gm2egm libpng-util libProtocols libEGM required-directories .FORCE

$(LIB_PFX)compileEGMf$(LIB_EXT): ENIGMA
ENIGMA: .FORCE libProtocols$(LIB_EXT) libENIGMAShared$(LIB_EXT)
	$(MAKE) -C CompilerSource

clean: .FORCE
	$(MAKE) -C CompilerSource/ clean
	$(MAKE) -C CommandLine/emake/ clean
	$(MAKE) -C CommandLine/libEGM/ clean
	$(MAKE) -C CommandLine/testing/ clean
	$(MAKE) -C shared/ clean
	$(MAKE) -C shared/protos/ clean
	$(MAKE) -C CommandLine/gm2egm/ clean

all: libENIGMAShared libProtocols libEGM ENIGMA gm2egm emake emake-tests test-runner .FORCE

Game: .FORCE
	@$(RM) -f logs/enigma_compile.log
	@$(MAKE) -C ENIGMAsystem/SHELL > >(tee -a /tmp/enigma_compile.log) 2> >(tee -a /tmp/enigma_compile.log >&2)

clean-game: .FORCE
	$(MAKE) -C ENIGMAsystem/SHELL clean

clean-protos: .FORCE
	$(MAKE) -C shared/protos/ clean

libpng-util: .FORCE
	$(MAKE) -C shared/libpng-util/

libENIGMAShared$(LIB_EXT): libENIGMAShared
libENIGMAShared: .FORCE libProtocols$(LIB_EXT)
	$(MAKE) -C shared/

libProtocols$(LIB_EXT): libProtocols
libProtocols: .FORCE
	$(MAKE) -C shared/protos/

libEGM$(LIB_EXT): libEGM
libEGM: .FORCE libProtocols$(LIB_EXT) libENIGMAShared$(LIB_EXT)
	$(MAKE) -C CommandLine/libEGM/

EMAKE_TARGETS = .FORCE ENIGMA libProtocols$(LIB_EXT) libEGM$(LIB_EXT)

emake: $(EMAKE_TARGETS) $(LIB_PFX)compileEGMf$(LIB_EXT)
	$(MAKE) -C CommandLine/emake/

emake-tests: .FORCE libEGM$(LIB_EXT) $(LIB_PFX)compileEGMf$(LIB_EXT)
	TESTS=TRUE $(MAKE) -C CommandLine/emake/

gm2egm: libEGM$(LIB_EXT) .FORCE
	$(MAKE) -C CommandLine/gm2egm/

test-runner: emake .FORCE
	$(MAKE) -C CommandLine/testing/

required-directories: .FORCE
	@mkdir -p "$(WORKDIR)"
	@mkdir -p "$(CODEGEN)/Preprocessor_Environment_Editable/"

.FORCE:
