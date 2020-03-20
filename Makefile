PATH := $(eTCpath)$(PATH)
SHELL=/bin/bash

.PHONY: ENIGMA all clean Game clean-game emake emake-tests gm2egm libpng-util libProtocols libEGM required-directories .FORCE

ENIGMA: .FORCE libProtocols libpng-util
	$(MAKE) -C CompilerSource

clean: .FORCE
	$(MAKE) -C CompilerSource/ clean
	$(MAKE) -C CommandLine/emake/ clean
	$(MAKE) -C CommandLine/libEGM/ clean
	$(MAKE) -C CommandLine/testing/ clean
	$(MAKE) -C shared/libpng-util/ clean
	$(MAKE) -C shared/protos/ clean
	$(MAKE) -C CommandLine/gm2egm/ clean

all: libpng-util libProtocols libEGM ENIGMA emake emake-tests test-runner .FORCE

Game: .FORCE
	@$(RM) -f logs/enigma_compile.log
	@$(MAKE) -C ENIGMAsystem/SHELL > >(tee -a /tmp/enigma_compile.log) 2> >(tee -a /tmp/enigma_compile.log >&2)

clean-game: .FORCE
	$(MAKE) -C ENIGMAsystem/SHELL clean

libpng-util: .FORCE
	$(MAKE) -C shared/libpng-util/

libProtocols: .FORCE
	$(MAKE) -C shared/protos/

libEGM: .FORCE libpng-util libProtocols
	$(MAKE) -C CommandLine/libEGM/

EMAKE_TARGETS = .FORCE

ifneq ($(CLI_ENABLE_EGM), FALSE)
	EMAKE_TARGETS += libEGM
else
	EMAKE_TARGETS += libProtocols
endif

emake: $(EMAKE_TARGETS)
	$(MAKE) -C CommandLine/emake/

emake-tests: .FORCE libEGM
	TESTS=TRUE $(MAKE) -C CommandLine/emake/

gm2egm: libEGM .FORCE
	$(MAKE) -C CommandLine/gm2egm/

test-runner: emake .FORCE
	$(MAKE) -C CommandLine/testing/

required-directories: .FORCE
	@mkdir -p "$(WORKDIR)"
	@mkdir -p "$(CODEGEN)Preprocessor_Environment_Editable/"

.FORCE:
