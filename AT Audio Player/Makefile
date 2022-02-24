SHELL = /bin/sh
CXX := g++
CXXFLAGS := -std=c++17 -Wall -g -I./src/header/ -I./src/header/external/ -I./src/header/external/ -I/home/isak/devdoc/repos/AT-Audio-Player/irrKlang-64bit-1.6.0/include -L/home/isak/devdoc/repos/AT-Audio-Player/irrKlang-64bit-1.6.0/bin/linux-gcc-64 -L/home/isak/devdoc/repos/AT-Audio-Player/irrKlang-64bit-1.6.0/bin/linux-gcc-64
# -std=c++17  	- Use C++17 standard
# -Wall  		- turns on most compiler warnings
# -g     		- adds debug information to the executable

srcdir := .\/src\/
bindir := .\/bin\/
intermediatedir := .\/src\/
#intermediatedir := .\/bin\/intermediates\/

APPNAME := atap
INCLUDES := -I./src/header/ -I./src/header/external/ -I/home/isak/devdoc/repos/AT-Audio-Player/irrKlang-64bit-1.6.0/include
LDLIBS := -lncurses /home/isak/devdoc/repos/AT-Audio-Player/irrKlang-64bit-1.6.0/bin/linux-gcc-64/libIrrKlang.so

srcfiles := $(shell find $(srcdir) -name "*.cpp")
objects := $(shell echo $(srcfiles) | sed s/"$(srcdir)"/"$(intermediatedir)"/g | sed s/".cpp"/".o"/g)


#Build the app specified in APPNAME for the "all" or "default" target
all: $(APPNAME)
default: $(APPNAME)

echo: 									#Print autodetected source and intermediate files
	@echo -e "\nSource files"
	@echo $(srcfiles)
	@echo -e "\nIntermediate files"
	@echo $(objects)
	@echo -e ""

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f *~ .depend
	rm -f $(bindir)$(APPNAME)

$(APPNAME): $(objects)
	$(CXX) ${INCLUDES} $(CXXFLAGS)  $(LDFLAGS) -o $(bindir)$(APPNAME) $(objects) $(LDLIBS)

depend: .depend

.depend: ${srcfiles}
	rm -f ./.depend
	$(CXX) ${INCLUDES} $(CXXFLAGS) -MM $(srcfiles)>>./.depend;


include .depend
