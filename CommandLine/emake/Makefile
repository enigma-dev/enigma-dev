CXX=g++
CXXFLAGS=-g -std=c++11 -c -Wall -I. -I../../CompilerSource

ifeq ($(OS),Windows_NT)
	LDFLAGS=-lboost_system-mt -Wl,--no-as-needed -Wl,-rpath=./ -lboost_program_options-mt -lboost_iostreams-mt -lboost_filesystem-mt -lboost_system-mt -lpthread
else
	LDFLAGS=-lboost_system -Wl,--no-as-needed -Wl,-rpath=./ -lboost_program_options -lboost_iostreams -lboost_filesystem -lboost_system -lpthread -ldl
endif
	
SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(addprefix .eobjs/,$(SOURCES:.cpp=.o))
EXECUTABLE=../../emake

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) eyaml
	$(CXX) $(OBJECTS) $(LDFLAGS) .eobjs/eyaml.o -o $@

.eobjs/%.o : %.cpp dir
	$(CXX) $(CXXFLAGS) $< -o $@

#I'm To lazy to write my own yaml parser or fix plugin to do shit I need
eyaml:
	$(CXX) $(CXXFLAGS) ../../CompilerSource/settings-parse/eyaml.cpp -o .eobjs/eyaml.o
	
dir:
	mkdir -p .eobjs/

clean:
	rm -f .eobjs/*.o
	rm -f ../../emake
