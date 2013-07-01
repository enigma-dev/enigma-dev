CXXFLAGS := -I. -I../ -Ibackend -I../CompilerSource -I./file -I./file/EGM -I./file/GMK -I./file/GMX -I./file/GMZ

SOURCES := $(wildcard *.cpp) $(wildcard backend/*.cpp)

LDLIBS := -ldl -lzip

OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))

all: ${OBJECTS}
	${CXX} ${OBJECTS} ${LDLIBS} -o ../enigmacli
clean:
	find . -name "*.o" -exec rm {} \;

%.o: %.cpp ${CXX} ${CXXFLAGS} -c $< -o $@
