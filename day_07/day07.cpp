#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <limits>

// Just brute force it

void part1(const std::vector<long>& h_positions);
void part2(const std::vector<long>& h_positions);
long fuel(const std::vector<long>& h_positions, long positions);
long fuel_v2(const std::vector<long>& h_positions, long position);
long max(const std::vector<long>& h_positions);
long min(const std::vector<long>& h_positions);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <input-filename>" << std::endl;
        std::exit(-1);
    }

    if (!std::filesystem::exists(argv[1])) {
        std::cerr << "error: could not find file named '" << argv[1] << "'" 
            << std::endl;
        std::exit(-1);
    }

    std::string input_filename = argv[1];
    std::ifstream inputfile;
    inputfile.open(input_filename);

    std::string line;
    std::getline(inputfile, line);

    size_t comma_pos;
    std::vector<long> h_positions; // horizontal positions of crabs
    while ((comma_pos = line.find(',')) != std::string::npos) {
        h_positions.push_back(std::stol(line.substr(0, comma_pos)));
        line = line.substr(comma_pos + 1, line.length());
    }

    if (line.length() != 0) { // process the last remaining number
        h_positions.push_back(std::stol(line));
    }

    part1(h_positions);
    part2(h_positions);

    inputfile.close();
}

void part1(const std::vector<long>& h_positions) {
    long first_sub = min(h_positions);
    long last_sub = max(h_positions);

    std::vector<long> costs;
    costs.reserve(last_sub - first_sub + 1);

    for (long i = first_sub; i <= last_sub; i++) {
        costs.push_back(fuel(h_positions, i));
    }

    std::cout << min(costs) << std::endl;
}

void part2(const std::vector<long>& h_positions) {
    long first_sub = min(h_positions);
    long last_sub = max(h_positions);

    std::vector<long> costs;
    costs.reserve(last_sub - first_sub + 1);

    for (long i = first_sub; i <= last_sub; i++) {
        costs.push_back(fuel_v2(h_positions, i));
    }

    std::cout << min(costs) << std::endl;
}

long fuel_v2(const std::vector<long>& h_positions, long position) {
    long fuel = 0;
    for (auto p : h_positions) {
        long distance = std::abs(p - position);
        fuel += (distance * (distance + 1)) / 2;
    }
    return fuel;
}

long fuel(const std::vector<long>& h_positions, long positions) {
    long fuel = 0;
    for (auto p : h_positions) {
        fuel += std::abs(p - positions);
    }
    return fuel;
}

long max(const std::vector<long>& h_positions) {
    long max = std::numeric_limits<long>::min();
    for (auto p : h_positions) {
        if (p >= max) {
            max = p;
        }
    }
    return max;
}

long min(const std::vector<long>& h_positions) {
    long min = std::numeric_limits<long>::max();
    for (auto p : h_positions) {
        if (p <= min) {
            min = p;
        }
    }
    return min;
}
