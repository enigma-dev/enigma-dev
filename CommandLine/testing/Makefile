include ../../Config.mk

TARGET := ../../test-runner
TESTCASES_DIR := $(SRC_DIR)/Tests

ifeq ($(OS), Darwin)
	OS_SRCS=./Platform/TestHarness-Cocoa.cpp
	OS_LIBS=-lstdc++fs
else ifeq ($(UNIX_BASED), true)
	OS_SRCS=$(wildcard ./Platform/*-X11.cpp)
	OS_LIBS=$(shell pkg-config x11 --libs) -lstdc++fs
else
	OS_SRCS=./Platform/TestHarness-Win32.cpp
endif

SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(TESTCASES_DIR)/*.cpp) $(OS_SRCS)
LDFLAGS += $(OS_LIBS) -lgtest -lpthread -lgtest_main

include ../../Default.mk
