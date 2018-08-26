.PHONY: liblodepng.a clean .FORCE

liblodepng.a: .FORCE
	mkdir -p .eobjs/
	$(CXX) $(subst -ftest-coverage,, $(CXXFLAGS)) -fPIC -MMD -c -o .eobjs/lodepng.o lodepng.cpp
	$(AR) rvs liblodepng.a .eobjs/lodepng.o

clean:
	rm -f .eobjs/lodepng.o liblodepng.a
