CXXFLAGS=-Wall -Werror -Wextra

CXX=g++

day02: day02.o
	$(CXX) $(CXXFLAGS) -o day02 day02.o

day02.o: day02.cpp
	$(CXX) $(CXXFLAGS) -c day02.cpp -o day02.o

clean:
	rm day02.o day02
