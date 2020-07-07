include ../../Config.mk

SHARED_SRC_DIR := ../../shared

PROTO_DIR := $(SHARED_SRC_DIR)/protos/.eobjs

ifeq ($(OS), Linux)
	OS_LIBS=-lboost_program_options -Wl,--no-as-needed -Wl,-rpath,./ -lpthread
else ifeq ($(OS), Darwin)
	OS_LIBS=-lboost_program_options -lpthread
else
	OS_LIBS=-lboost_system-mt -Wl,--no-as-needed -Wl,-rpath,./ -lboost_program_options-mt -lpthread
endif

CXXFLAGS  += -I../../CompilerSource -I$(PROTO_DIR)
LDFLAGS   += $(OS_LIBS) -L../../ -lcompileEGMf -lProtocols -lENIGMAShared -lprotobuf -lpng

ifeq ($(TESTS), TRUE)
	TARGET=../../emake-tests
	SOURCES := $(call rwildcard, ../libEGM-test,*.cpp)
	LDFLAGS += -lgtest -lpthread -lgtest_main
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

ifeq ($(OS), Darwin)
	LINKING_TARGETS += make AppleFixLinks
endif

AppleFixLinks:
	install_name_tool -change ../../libProtocols.dylib @loader_path/./libProtocols.dylib ../../libEGM.dylib
	install_name_tool -change ../../libProtocols.dylib @loader_path/./libProtocols.dylib ../../libcompileEGMf.dylib
	install_name_tool -change ../libENIGMAShared.dylib @loader_path/./libENIGMAShared.dylib ../../libcompileEGMf.dylib
	install_name_tool -change ../../libEGM.dylib @loader_path/./libEGM.dylib $(TARGET)
	install_name_tool -change ../libcompileEGMf.dylib @loader_path/./libcompileEGMf.dylib $(TARGET)
	install_name_tool -change ../libENIGMAShared.dylib @loader_path/./libENIGMAShared.dylib $(TARGET)
	install_name_tool -change ../../libProtocols.dylib @loader_path/./libProtocols.dylib $(TARGET)
	
include ../../Default.mk
