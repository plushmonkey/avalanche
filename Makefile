CXXFLAGS=-std=c++14 -fPIC -O2 -I/usr/include/jsoncpp -Ilib/mclib/mclib/include
LIBS=-L. -ljsoncpp -lmc
CXX=clang++
BIN=bin

SRC=$(shell find avalanche -type f -name *.cpp)

.PHONY: clean
all: bin libmc.so avalanche
avalanche: $(SRC:.cpp=.o)
	$(CXX) -o $(BIN)/$@ $(CXXFLAGS) $^ -Wl,-rpath,. $(LIBS)
	-mv -f libmc.so bin/libmc.so

libmc.so:
	$(MAKE) -C lib/mclib
	cp lib/mclib/libmc.so libmc.so

bin:
	-mkdir $(BIN)

clean:
	-rm -f $(SRC:.cpp=.o)
