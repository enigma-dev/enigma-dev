include ../../Config.mk

TARGET := ../../test-runner
TESTCASES_DIR := $(SRC_DIR)/Tests
SHARED_SRC_DIR := ../../shared

# VISUAL_SHADER_OBJ_DIR := $(SHARED_SRC_DIR)/.eobjs/shared/ResourceTransformations/VisualShader

ifeq ($(OS), Darwin)
	OS_SRCS=./Platform/TestHarness-Cocoa.cpp
else ifeq ($(UNIX_BASED), true)
	OS_SRCS=$(wildcard ./Platform/*-X11.cpp)
	OS_LIBS:=$(shell pkg-config x11 --libs)
	ifeq ($(OS), Linux)
		OS_LIBS+=-lstdc++fs
	endif
else
	OS_SRCS=./Platform/TestHarness-Win32.cpp
endif

# CXXFLAGS += -I$(VISUAL_SHADER_OBJ_DIR)
SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(TESTCASES_DIR)/*.cpp) $(OS_SRCS) $(wildcard ../../shared/ResourceTransformations/VisualShader/*.cpp)
LDFLAGS += $(OS_LIBS) -lgtest_main -lgtest -lpthread

include ../../Default.mk
