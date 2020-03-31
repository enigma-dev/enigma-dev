include ../../Config.mk

TARGET := ../../gm2egm

SHARED_SRC_DIR := ../../shared
CXXFLAGS += -I../../shared/protos/ -I../../shared/protos/.eobjs -I../libEGM/
LDFLAGS  += -Wl,-rpath,./ -L../../ -lEGM -lProtocols -lENIGMAShared -lyaml-cpp -lpthread
SOURCES  += main.cpp

include ../../Default.mk
