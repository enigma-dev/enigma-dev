include ../../Config.mk

TARGET := ../../gmk-checker

SHARED_SRC_DIR := ../../shared
BREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)
CXXFLAGS += -I../../shared/protos/ -I../../shared/protos/.eobjs -I../libEGM/ -I$(BREW_PREFIX)/include $(PROTOBUF_CFLAGS) $(YAML_CFLAGS)
LDFLAGS  += -Wl,-rpath,@executable_path -L../../ -lEGM -lProtocols -lENIGMAShared $(PROTOBUF_LDFLAGS) $(PROTOBUF_LIBS) $(YAML_LDFLAGS) $(YAML_LIBS) -lpthread
SOURCES  += main.cpp

ifeq ($(OS),Darwin)
  # List the dynamic libraries that need their paths fixed
  DYLIBS_TO_FIX = \
    ../../libEGM.dylib \
    ../../libProtocols.dylib \
    ../../libENIGMAShared.dylib \
    ../libENIGMAShared.dylib

  # Build the command string, using foreach to loop through the list
  # Note: The backslashes (\) and semicolon (;) are needed to make this one single shell command.
  DYLIB_FIX_COMMANDS = $(foreach lib,$(DYLIBS_TO_FIX),\
    install_name_tool -change "$(lib)" "@executable_path/$(notdir $(lib))" $@ \
    || ( rm -f $@ && false ) ; \
  )

  # (Optional) Add a message for clarity during the build process
  DYLIB_FIX_COMMANDS += echo "Successfully fixed dylib paths for $@"
endif

include ../../Default.mk
