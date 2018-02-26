liblodepng.a:
	mkdir -p .eobjs/
	g++ -fPIC -MMD -c -o .eobjs/lodepng.o lodepng.cpp
	ar rvs liblodepng.a .eobjs/lodepng.o
	
clean:
	rm .eobjs/lodepng.o liblodepng.a
