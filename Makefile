PATH := $(eTCpath)$(PATH)
SHELL=/bin/bash

.PHONY: ENIGMA all clean Game clean-game clean-protos libpng-util libProtocols libEGM required-directories .FORCE

ENIGMA: .FORCE libProtocols libpng-util
	$(MAKE) -C CompilerSource

clean: .FORCE
	$(MAKE) -C CompilerSource/ clean
	$(MAKE) -C CommandLine/emake/ clean
	$(MAKE) -C CommandLine/libEGM/ clean
	$(MAKE) -C CommandLine/testing/ clean
	$(MAKE) -C shared/libpng-util/ clean
	$(MAKE) -C shared/protos/ clean
	rm -f ./gm2egm

all: libpng-util libProtocols libEGM ENIGMA emake test-runner .FORCE

Game: .FORCE
	@$(RM) -f logs/enigma_compile.log
	@$(MAKE) -C ENIGMAsystem/SHELL > >(tee -a logs/enigma_compile.log) 2> >(tee -a logs/enigma_compile.log >&2)

clean-game: .FORCE
	$(MAKE) -C ENIGMAsystem/SHELL clean

clean-protos: .FORCE
	$(MAKE) -C shared/protos/ clean

libpng-util: .FORCE
	$(MAKE) -C shared/libpng-util/

libProtocols: .FORCE
	$(MAKE) -C shared/protos/

libEGM: .FORCE libpng-util libProtocols
	$(MAKE) -C CommandLine/libEGM/

StandAlone:
	echo hi

EMAKE_TARGETS = .FORCE

ifneq ($(CLI_ENABLE_EGM), FALSE)
	EMAKE_TARGETS += libEGM
else
	EMAKE_TARGETS += libProtocols
endif

emake: $(EMAKE_TARGETS)
	$(MAKE) -C CommandLine/emake/

gm2egm: libEGM .FORCE
	$(CXX) -Ishared/protos/ -Ishared/protos/.eobjs -ICommandLine/libEGM/ CommandLine/gm2egm/main.cpp -Wl,-rpath,. -L. -lEGM -lProtocols -o gm2egm

test-runner: emake .FORCE
	$(MAKE) -C CommandLine/testing/

required-directories: .FORCE
	@mkdir -p "$(WORKDIR)"
	@mkdir -p "$(CODEGEN)Preprocessor_Environment_Editable/"

.FORCE:
