.PHONY: libpng-util.a clean .FORCE

include ../../Default.mk

libpng-util.a: .FORCE
	mkdir -p .eobjs/
	$(CXX) $(subst -ftest-coverage,, $(CXXFLAGS)) -fPIC -MMD -c -o .eobjs/libpng-util.o libpng-util.cpp
	$(AR) rvs libpng-util.a .eobjs/libpng-util.o

clean:
	rm -f .eobjs/libpng-util.o libpng-util.a
