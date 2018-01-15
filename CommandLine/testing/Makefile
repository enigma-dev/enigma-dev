BINARY := ../../test-runner
SRC_DIR := .
OBJ_DIR := .eobjs
TESTCASES_DIR := $(SRC_DIR)/Tests
EMAKE := ../../emake

OS := $(shell uname -s)
ifeq ($(OS), Linux)
	OS_SRCS=./Platform/TestHarness-X11.cpp
	OS_LIBS=-lX11
else ifeq ($(OS), Darwin)
	OS_SRCS=./Platform/TestHarness-Cocoa.cpp
else
	OS_SRCS=./Platform/TestHarness-Win32.cpp
endif

CXXFLAGS  := -I$(SRC_DIR) -Wall -Wextra -Wpedantic -g
LDFLAGS   := $(OS_LIBS) -lgtest_main -lgtest

SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(TESTCASES_DIR)/*.cpp) $(OS_SRCS)
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))
OBJDIRS := $(sort $(OBJ_DIR) $(dir $(OBJECTS)))
DEPENDS := $(OBJECTS:.o=.d)

$(BINARY): $(OBJECTS)
	g++ $^ -o $@ $(LDFLAGS)

.PHONY: all clean

all: $(BINARY)
clean:
	rm -rf $(BINARY) $(OBJ_DIR)

# Create the object directories
$(OBJDIRS):
	mkdir -p $@

# Generate rules for new (unbuilt) files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJDIRS)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) -MMD -c -o $(OBJ_DIR)/$*.o $<

# Include rules for known (previously-built) files
-include $(DEPENDS)
.SUFFIXES:

# ==============================================================================
# == CUSTOM RULES ==============================================================
# ==============================================================================

$(EMAKE):
	$(MAKE) -C $(dir $(EMAKE)) $(notdir $(EMAKE))
