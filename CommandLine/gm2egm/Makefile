include ../../Config.mk

TARGET := ../../gm2egm

CXXFLAGS := -I../../shared/protos/ -I../../shared/protos/.eobjs -I../libEGM/
LDFLAGS  := -Wl,-rpath,. -L../../ -lEGM -lProtocols -lpthread
SOURCES  := main.cpp

include ../../Default.mk
