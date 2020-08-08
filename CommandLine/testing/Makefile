include ../../Config.mk

TARGET := ../../test-runner
TESTCASES_DIR := $(SRC_DIR)/Tests
SHARED_SRC_DIR := ../../shared

ifeq ($(OS), Linux)
	OS_SRCS=$(wildcard ./Platform/*-X11.cpp)
	OS_LIBS=-lX11 -lstdc++fs
else ifeq ($(OS), Darwin)
	OS_SRCS=./Platform/TestHarness-Cocoa.cpp
else
	OS_SRCS=./Platform/TestHarness-Win32.cpp
endif

SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(TESTCASES_DIR)/*.cpp) $(OS_SRCS)
LDFLAGS += $(OS_LIBS) -lgtest_main -lgtest -lpthread

include ../../Default.mk
