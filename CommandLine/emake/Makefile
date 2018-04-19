BINARY := ../../emake
SRC_DIR := .
OBJ_DIR := .eobjs

OS := $(shell uname -s)
ifeq ($(OS), Linux)
	OS_LIBS=-lboost_system -Wl,--no-as-needed -Wl,-rpath=./ -lboost_program_options -lboost_iostreams -lboost_filesystem -lboost_system -lpthread -ldl
else ifeq ($(OS), Darwin)
	OS_LIBS=-lboost_system -lboost_program_options -lboost_iostreams -lboost_filesystem -lboost_system -lpthread -ldl
else
	OS_LIBS=-lboost_system-mt -Wl,--no-as-needed -Wl,-rpath=./ -lboost_program_options-mt -lboost_iostreams-mt -lboost_filesystem-mt -lboost_system-mt -lpthread
endif

EXTRA_SOURCES := ../../CompilerSource/settings-parse/eyaml.cpp ../../CompilerSource/compiler/event_reader/event_parser.cpp
EXTRA_SRC_DIR := ../../CompilerSource

PROTO_DIR := ../protos
CXXFLAGS  := -I$(SRC_DIR) -I$(EXTRA_SRC_DIR) -I$(PROTO_DIR) -I$(PROTO_DIR)/codegen -std=c++11 -Wall -Wextra -Wpedantic -g
LDFLAGS   := $(OS_LIBS) -L../../ -lProtocols -lprotobuf

EXTRA_OBJS := $(patsubst $(EXTRA_SRC_DIR)/%, $(OBJ_DIR)/Extra/%, $(patsubst %.cpp, %.o, $(EXTRA_SOURCES)))

rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))
SOURCES := $(call rwildcard,$(SRC_DIR),*.cpp)
ifeq ($(TRAVIS), true)
	disable_server := true
endif
ifeq (_$(disable_egm),$(filter _$(disable_egm),_true _1 _yes _y))
	CXXFLAGS += -DCLI_DISABLE_EGM
else
	CXXFLAGS += -I../libEGM -I../libEGM
	LDFLAGS += -lEGM
endif
ifeq (_$(disable_server),$(filter _$(disable_server),_true _1 _yes _y))
	SOURCES := $(filter-out ./Server.cpp, $(SOURCES))
	CXXFLAGS += -DCLI_DISABLE_SERVER
else
	LDFLAGS += -lgrpc++
endif
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))
OBJDIRS := $(sort $(OBJ_DIR) $(dir $(OBJECTS)) $(dir $(EXTRA_OBJS)))
DEPENDS := $(OBJECTS:.o=.d) $(EXTRA_OBJS:.o=.d)

$(BINARY): $(OBJECTS) $(EXTRA_OBJS)
	g++ $^ $(LDFLAGS) -o $@

.PHONY: all clean obj_dirs

all: $(BINARY)

clean:
	rm -rf $(BINARY) $(OBJ_DIR)

# Create the object directories
$(OBJDIRS):
	mkdir -p $@
# Old make has a bug which requires this nonsense...
obj_dirs: $(OBJDIRS)

# Generate rules for new (unbuilt) files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<
$(OBJ_DIR)/Extra/%.o: $(EXTRA_SRC_DIR)/%.cpp | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

# Include rules for known (previously-built) files
-include $(DEPENDS)
.SUFFIXES:
