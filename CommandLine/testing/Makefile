include ../../Config.mk

TARGET := ../../test-runner
TESTCASES_DIR := $(SRC_DIR)/Tests

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

SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(TESTCASES_DIR)/*.cpp) $(OS_SRCS)
LDFLAGS += $(OS_LIBS) -lgtest -lpthread -lgtest_main

include ../../Default.mk
