#include <fstream>
#include <iostream>
#include <vector>

void part1(std::vector<int> &);
void part2(std::vector<int> &);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " " << "<inputfile>" << std::endl;
        std::exit(-1);
    }
    char *filename = argv[1];
    std::ifstream input;
    input.open(filename);

    std::string line;

    // vector to hold integer readings
    std::vector<int> readings;

    // parse and populate readings
    while (std::getline(input, line)) {
        if (line.size() != 0) readings.push_back(std::stoi(line));
    }

    part1(readings);
    part2(readings);
}

void part1(std::vector<int>& readings) {
    size_t prev = -1; // prev <- 2**64 - 1 // as a safety in the first iteration of 
                      // the loop below
    int count = 0;

    for (std::vector<int>::iterator i = readings.begin(); i != readings.end();
         i++) {
        size_t current = *i;
        if (prev != (size_t) -1 && current > prev)  count++; 
        prev = current;
    }

    std::cout << count << std::endl;
}

void part2(std::vector<int> &readings) {
    size_t window = 3;
    size_t prev = -1; // prev <- 2**64 - 1 // as a safety in the first iteration of 
                      // the loop below
    int count = 0;
    for(size_t i = 0; i <= readings.size() - window; i++) {
        // sum and store window elements in inner_sum
        size_t inner_sum = 0;
        for(size_t j = 0; j < window; j++) {
            inner_sum += readings[i + j];
        }
        if (prev != (size_t) -1 && inner_sum > prev) count++;

        prev = inner_sum;
    }
    std::cout << count << std::endl;
}
