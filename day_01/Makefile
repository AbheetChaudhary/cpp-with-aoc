CXXFLAGS=-Wall -Werror -Wextra

CXX=g++

day01: day01.o
	$(CXX) $(CXXFLAGS) -o day01 day01.o

day01.o: day01.cpp
	$(CXX) $(CXXFLAGS) -c day01.cpp -o day01.o

clean:
	rm day01.o day01
