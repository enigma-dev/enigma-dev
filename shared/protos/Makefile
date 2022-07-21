include ../../Config.mk

PROTO_SOURCES := $(wildcard *.proto) $(wildcard Configuration/*.proto)
SOURCES_GRPC := server.proto
LDFLAGS += -lgrpc++ -lgrpc -laddress_sorting -lre2 -lupb -lcares -lz -lgpr -lssl -lcrypto -labsl_raw_hash_set -labsl_hashtablez_sampler -labsl_hash -labsl_city -labsl_low_level_hash -labsl_statusor -labsl_status -labsl_cord -labsl_cordz_info -labsl_cord_internal -labsl_cordz_functions -labsl_exponential_biased -labsl_cordz_handle -labsl_bad_optional_access -labsl_str_format_internal -labsl_synchronization -labsl_graphcycles_internal -labsl_stacktrace -ldbghelp -labsl_symbolize -labsl_debugging_internal -labsl_demangle_internal -labsl_malloc_internal -labsl_time -labsl_civil_time -labsl_strings -labsl_strings_internal -ladvapi32 -labsl_base -labsl_spinlock_wait -labsl_int128 -labsl_throw_delegate -labsl_time_zone -labsl_bad_variant_access -labsl_raw_logging_internal -labsl_log_severity
CXXFLAGS += -D_WIN32_WINNT=0x0600
GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin)$(BIN_EXT)

TARGET := ../../libProtocols$(LIB_EXT)

CXXFLAGS += -I$(OBJ_DIR) -fPIC
LDFLAGS += -shared -lprotobuf

GENERATED := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.cc) $(PROTO_SOURCES:.proto=.pb.h) $(SOURCES_GRPC:.proto=.grpc.pb.cc) $(SOURCES_GRPC:.proto=.grpc.pb.h))
OBJECTS := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.o) $(SOURCES_GRPC:.proto=.grpc.pb.o))

include ../../Default.mk


