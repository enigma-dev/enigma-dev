.PHONY: libpng-util.a clean .FORCE

# I tried include ../../Default.mk and it had a warning saying overriding .FORCE
OS := $(shell uname -s)
ifeq ($(OS), FreeBSD)
	CXXFLAGS += -I/usr/local/include
	CFLAGS   += -I/usr/local/include
	LDFLAGS  += -L/usr/local/lib
endif

libpng-util.a: .FORCE
	mkdir -p .eobjs/
	$(CXX) $(subst -ftest-coverage,, $(CXXFLAGS)) -fPIC -MMD -c -o .eobjs/libpng-util.o libpng-util.cpp
	$(AR) rvs libpng-util.a .eobjs/libpng-util.o

clean:
	rm -f .eobjs/libpng-util.o libpng-util.a
