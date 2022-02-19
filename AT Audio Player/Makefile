appname := atap

CXX := g++
CXXFLAGS := -std=c++17 -Wall -g -I ./src/header/ -I ./src/header/external/ -I /home/isak/devdoc/repos/AT-Audio-Player/irrKlang-64bit-1.6.0/include -I ./ConTerLib/include -L /home/isak/devdoc/repos/AT-Audio-Player/irrKlang-64bit-1.6.0/bin/linux-gcc-64 /home/isak/devdoc/repos/AT-Audio-Player/irrKlang-64bit-1.6.0/bin/linux-gcc-64/libIrrKlang.so
# -Wall  - turns on most compiler warnings
# -g     - adds debug information to the executable

INCLUDES := "-I./src/header/"
srcfiles := $(shell find ./src/ -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) -I. $(CXXFLAGS) ${INCLUDES} $(LDFLAGS) -o $(appname) $(objects) ./ConTerLib/src/conterlib.cpp $(LDLIBS)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) -I. $(CXXFLAGS) ${INCLUDES} -MM $^>>./.depend;

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f *~ .depend

include .depend
