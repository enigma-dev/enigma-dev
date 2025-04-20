include ../../Config.mk

TARGET := ../../gm2egm

SHARED_SRC_DIR := ../../shared
CXXFLAGS += -I../../shared/protos/ -I../../shared/protos/.eobjs -I../libEGM/
LDFLAGS  += -Wl,-rpath,./ -L../../ -lEGM$(SUFFIX) -lProtocols$(SUFFIX) -lENIGMAShared$(SUFFIX) -lyaml-cpp -lpthread -lprotobuf
SOURCES  += main.cpp

include ../../Default.mk
