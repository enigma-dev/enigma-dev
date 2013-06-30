CXXFLAGS := -I. -I../ -Ibackend -I../CompilerSource

SOURCES := $(wildcard *.cpp) $(wildcard backend/*.cpp)

LDLIBS := -ldl

OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))

all: ${OBJECTS}
	${CXX} ${OBJECTS} ${LDLIBS} -o enigmacli
clean:
	find . -name "*.o" -exec rm {} \;

%.o: %.cpp ${CXX} ${CXXFLAGS} -c $< -o $@
