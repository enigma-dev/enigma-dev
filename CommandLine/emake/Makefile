include ../../Config.mk

SHARED_SRC_DIR := ../../shared
include $(SHARED_SRC_DIR)/Makefile
include $(SHARED_SRC_DIR)/eyaml/Makefile
include $(SHARED_SRC_DIR)/ProtoYaml/Makefile
include $(SHARED_SRC_DIR)/event_reader/Makefile

PROTO_DIR := $(SHARED_SRC_DIR)/protos/.eobjs

ifeq ($(OS), Linux)
	OS_LIBS=-lboost_system -Wl,--no-as-needed -Wl,-rpath,./ -lboost_program_options -lboost_iostreams -lboost_filesystem -lboost_system -lpthread -ldl
else ifeq ($(OS), Darwin)
	OS_LIBS=-lboost_system -lboost_program_options -lboost_iostreams -lboost_filesystem -lboost_system -lpthread -ldl
else
	OS_LIBS=-lboost_system-mt -Wl,--no-as-needed -Wl,-rpath,./ -lboost_program_options-mt -lboost_iostreams-mt -lboost_filesystem-mt -lboost_system-mt -lpthread
endif

CXXFLAGS  += -I../../CompilerSource -I$(PROTO_DIR)
LDFLAGS   += $(OS_LIBS) -L../../ -lProtocols -lprotobuf -lyaml-cpp

ifeq ($(TESTS), TRUE)
	TARGET=../../emake-tests
	SOURCES := $(call rwildcard, ../emake-tests,*.cpp)
	LDFLAGS += -lpthread -lgtest_main -lgtest
else
	TARGET = ../../emake
	SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
endif

#ON BY DEFAULT
ifneq ($(CLI_ENABLE_EGM), FALSE)
	CXXFLAGS += -DCLI_ENABLE_EGM
	CXXFLAGS += -I../libEGM -I../libEGM
	LDFLAGS += -lEGM
endif

#OFF BY DEFAULT
ifeq ($(CLI_ENABLE_SERVER), TRUE)
	CXXFLAGS += -DCLI_ENABLE_SERVER
	LDFLAGS += -lgrpc++
else
	SOURCES := $(filter-out ./Server.cpp, $(SOURCES))
endif

include ../../Default.mk
