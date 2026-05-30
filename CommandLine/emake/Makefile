include ../../Config.mk

SHARED_SRC_DIR := ../../shared

PROTO_DIR := $(SHARED_SRC_DIR)/protos/$(OBJ_DIR)

ifeq ($(OS), Darwin)
	OS_LIBS=-lboost_program_options -lpthread -ldl -lgmock
else ifeq ($(UNIX_BASED), true)
	OS_LIBS=-lboost_program_options -Wl,--no-as-needed -Wl,-rpath,./ -lpthread -ldl -lgmock
else
	OS_LIBS=-lboost_program_options-mt -Wl,--no-as-needed -Wl,-rpath,./ -lpthread -lgmock
endif

CXXFLAGS  += -I../../CompilerSource -I../../CompilerSource/JDI/src -I$(PROTO_DIR) -I../libEGM -I../libEGM -I../../ENIGMAsystem/SHELL $(GRPC_CFLAGS) $(PROTOBUF_CFLAGS) $(PUGI_CFLAGS) $(YAML_CFLAGS) $(PNG_CFLAGS) $(ZLIB_CFLAGS) $(FALLBACK_INC)
LDFLAGS   += $(FALLBACK_LIB_PATH) $(FALLBACK_LDFLAGS_EXTRA) $(OS_LIBS) -L../../ -lcompileEGMf -lEGM -lENIGMAShared -lProtocols $(GRPC_LDFLAGS) $(GRPC_LIBS) $(PROTOBUF_LDFLAGS) $(PROTOBUF_LIBS) $(PUGI_LDFLAGS) $(PUGI_LIBS) $(YAML_LDFLAGS) $(YAML_LIBS) $(PNG_LDFLAGS) $(PNG_LIBS) $(ZLIB_LDFLAGS) $(ZLIB_LIBS)

ifneq ($(CUSTOM_LIB_SEARCH_PRIORITY), system)
	LDFLAGS += -labseil_dll
endif

ifeq ($(TESTS), TRUE)
	TARGET=../../emake-tests
	SOURCES := $(filter-out ../emake-tests/asan_stubs.cpp,$(call rwildcard,../emake-tests,*.cpp)) \
	           $(call rwildcard,../emake-tests/Extensions/Json,*.cpp) \
	           ../emake-tests/asan_stubs.cpp
	LDFLAGS += -lpthread -lgtest_main -lgtest
else
	TARGET = ../../emake
	SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
endif

ifeq ($(OS),Darwin)
  # List the dynamic libraries that need their paths fixed
  DYLIBS_TO_FIX = \
    ../libcompileEGMf.dylib \
    ../../libEGM.dylib \
    ../libENIGMAShared.dylib \
    ../../libProtocols.dylib

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

# Clean object files from emake-tests directory when cleaning tests
ifeq ($(TESTS), TRUE)
clean: .FORCE
	rm -rf $(TARGET) $(OBJ_DIR) .eobjs-asan
	find ../emake-tests -name "*.o" -type f -delete 2>/dev/null || true
endif
