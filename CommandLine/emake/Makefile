include ../../Config.mk

SHARED_SRC_DIR := ../../shared

PROTO_DIR := $(SHARED_SRC_DIR)/protos/.eobjs

ifeq ($(OS), Darwin)
	OS_LIBS=-lboost_program_options -lpthread -ldl
else ifeq ($(UNIX_BASED), true)
	OS_LIBS=-lboost_program_options -Wl,--no-as-needed -Wl,-rpath,./ -lpthread -ldl
else
	OS_LIBS=-lboost_system-mt -Wl,--no-as-needed -Wl,-rpath,./ -lboost_program_options-mt -lpthread
endif

CXXFLAGS  += -I../../CompilerSource -I$(PROTO_DIR) -I../libEGM -I../libEGM -I../../ENIGMAsystem/SHELL
LDFLAGS   += $(OS_LIBS) -L../../ -lcompileEGMf -lEGM -lENIGMAShared -lProtocols -lgrpc++ -lgpr -lprotobuf -lyaml-cpp -lpng

ifneq ($(CUSTOM_LIB_SEARCH_PRIORITY), system)
	LDFLAGS += -labseil_dll
endif

ifeq ($(TESTS), TRUE)
	TARGET=../../emake-tests
	SOURCES := $(call rwildcard, ../emake-tests,*.cpp) $(call rwildcard, ../emake-tests/Extensions/Json,*.cpp)
	LDFLAGS += -lpthread -lgtest_main -lgtest
else
	TARGET = ../../emake
	SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
endif

include ../../Default.mk
