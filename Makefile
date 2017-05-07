CXXFLAGS=-std=c++14 -Wall -fPIC -O2 -I/usr/include/jsoncpp -Ilib/mclib/mclib/include
LIBS=-L. -ljsoncpp -lmc
CXX=clang++
BIN=bin

SRC=$(shell find avalanche -type f -name *.cpp)

avalanche: $(SRC:.cpp=.o) libmc.so
	$(CXX) -o $(BIN)/$@ $(CXXFLAGS) $^ -Wl,-rpath,. $(LIBS)
	-mv -f libmc.so bin/libmc.so

libmc.so: directory
	$(MAKE) -C lib/mclib
	cp lib/mclib/libmc.so libmc.so

directory:
	-mkdir $(BIN)

clean:
	-rm -f avalanche/*.o
	-rm -f avalanche/*/*.o
