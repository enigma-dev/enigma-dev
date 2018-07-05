BINARY := ../../test-runner
SRC_DIR := .
OBJ_DIR := .eobjs
TESTCASES_DIR := $(SRC_DIR)/Tests

OS := $(shell uname -s)
ifeq ($(OS), Linux)
	OS_SRCS=$(wildcard ./Platform/*-X11.cpp)
	OS_LIBS=-lX11 -lboost_filesystem -lboost_system
else ifeq ($(OS), Darwin)
	OS_SRCS=./Platform/TestHarness-Cocoa.cpp
	OS_LIBS=-lboost_filesystem -lboost_system
else
	OS_SRCS=./Platform/TestHarness-Win32.cpp
	OS_LIBS=-lboost_filesystem-mt -lboost_system-mt
endif

CXXFLAGS  := -I$(SRC_DIR) -Wall -Wextra -Wpedantic -g -std=c++11
LDFLAGS   := $(OS_LIBS) -lgtest -lpthread

SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(TESTCASES_DIR)/*.cpp) $(OS_SRCS)
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))
OBJDIRS := $(sort $(OBJ_DIR) $(dir $(OBJECTS)))
DEPENDS := $(OBJECTS:.o=.d)

$(BINARY): $(OBJECTS)
	g++ $^ -o $@ $(LDFLAGS)

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
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) -MMD -c -o $(OBJ_DIR)/$*.o $<

# Include rules for known (previously-built) files
-include $(DEPENDS)
.SUFFIXES:
