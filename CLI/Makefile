CXXFLAGS := -I. -I../ -Ibackend -I../CompilerSource

SOURCES := $(wildcard *.cpp) $(wildcard backend/*.cpp)

LDLIBS := -ldl

OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))

all: ${OBJECTS}
%.o: %.cpp ${CXX} ${CXXFLAGS} -c $< -o $@
