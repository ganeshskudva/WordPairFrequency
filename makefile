CXX=g++
CPPFLAGS=-Wall -IfileIOs_wordPairs.h -g
all:
	$(CXX) $(CPPFLAGS) -o sample fileIOs_wordPairs.cpp fileIOswordPairsmain.cpp --std=c++11

clean:
	rm -rf sample 