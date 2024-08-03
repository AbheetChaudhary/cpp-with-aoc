CXXFLAGS=-Wall -Werror -Wextra

CXX=g++

day03: day03.o
	$(CXX) $(CXXFLAGS) -o day03 day03.o

day03.o: day03.cpp
	$(CXX) $(CXXFLAGS) -c day03.cpp -o day03.o

clean:
	rm day03.o day03
