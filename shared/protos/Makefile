include ../../Config.mk

PROTO_SOURCES := $(wildcard *.proto) $(wildcard Configuration/*.proto)
SOURCES_GRPC := server.proto

LDFLAG += $(shell pkg-config --cflags --libs protobuf grpc grpc++)

ifeq ($(UNIX_BASED), true
	GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin)
else
	FOOBAR := -pthread -DCARES_STATICLIB -DNOMINMAX -lprotobuf -lgrpc++ -lgrpc -lgpr -labsl_flags -labsl_flags_internal -labsl_flags_reflection -labsl_flags_private_handle_accessor -labsl_flags_commandlineflag -labsl_flags_commandlineflag_internal -labsl_flags_config -labsl_flags_program_name -labsl_flags_marshalling -labsl_raw_hash_set -labsl_hashtablez_sampler -labsl_hash -labsl_city -labsl_low_level_hash -labsl_random_distributions -labsl_random_seed_sequences -labsl_random_internal_pool_urbg -labsl_random_internal_randen -labsl_random_internal_randen_hwaes -labsl_random_internal_randen_hwaes_impl -labsl_random_internal_randen_slow -labsl_random_internal_platform -lbcrypt -labsl_random_internal_seed_material -labsl_random_seed_gen_exception -labsl_statusor -labsl_status -labsl_cord -labsl_cordz_info -labsl_cord_internal -labsl_cordz_functions -labsl_exponential_biased -labsl_cordz_handle -labsl_crc_cord_state -labsl_crc32c -labsl_crc_internal -labsl_crc_cpu_detect -labsl_bad_optional_access -labsl_strerror -labsl_str_format_internal -labsl_synchronization -labsl_graphcycles_internal -labsl_stacktrace -ldbghelp -labsl_symbolize -labsl_debugging_internal -labsl_demangle_internal -labsl_malloc_internal -labsl_time -labsl_civil_time -labsl_strings -labsl_strings_internal -ladvapi32 -labsl_base -labsl_spinlock_wait -labsl_int128 -labsl_throw_delegate -labsl_time_zone -labsl_bad_variant_access -labsl_raw_logging_internal -labsl_log_severity
	GRPC_CPP_PLUGIN_PATH ?= $(shell which grpc_cpp_plugin).exe
	CXXFLAGS += -D_WIN32_WINNT=0x0600
endif

TARGET := ../../libProtocols$(LIB_EXT)

CXXFLAGS += -I$(OBJ_DIR) -fPIC

LDFLAG += -shared

ifeq ($(UNIX_BASED), true)
	GENERATED := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.cc) $(PROTO_SOURCES:.proto=.pb.h) $(SOURCES_GRPC:.proto=.grpc.pb.cc) $(SOURCES_GRPC:.proto=.grpc.pb.h))
	OBJECTS := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.o) $(SOURCES_GRPC:.proto=.grpc.pb.o))
else
	GENERATED := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.cc) $(PROTO_SOURCES:.proto=.pb.h) $(SOURCES_GRPC:.proto=.grpc.pb.cc) $(SOURCES_GRPC:.proto=.grpc.pb.h))
	OBJECTS := $(addprefix $(OBJ_DIR)/,$(PROTO_SOURCES:.proto=.pb.o) $(SOURCES_GRPC:.proto=.grpc.pb.o)) $(FOOBAR)
endif

include ../../Default.mk
