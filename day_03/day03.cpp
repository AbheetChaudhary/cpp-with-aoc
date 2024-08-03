#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>    // std::remove_if

struct BitCount {
    size_t zero_count;
    size_t one_count;
};

void part1(std::vector<BitCount>&);
void part2(std::vector<std::string>&);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <inputfile>" << std::endl;
        std::exit(-1);
    }

    char* filename = argv[1];
    std::ifstream inputfile;
    inputfile.open(filename);

    std::string line;

    std::vector<std::string> binary_readings;


    while(std::getline(inputfile, line)) {
        if (line.size() != 0) binary_readings.push_back(line);
    }

    /*
    for (auto& l : binary_readings) {
        std::cout << l << std::endl;
    }
    */

    std::vector<BitCount> bit_counts;
    bit_counts.reserve(binary_readings[0].size());
    for (size_t i = 0; i < bit_counts.capacity(); i++) {
        bit_counts.push_back(BitCount{ .zero_count = 0,
                .one_count = 0 });
    }

    for (std::vector<std::string>::iterator s = binary_readings.begin();
            s != binary_readings.end(); s++) {
        for (size_t i = 0; i < (*s).size(); i++) {
            if ((*s)[i] == '0') {
                bit_counts[i].zero_count++;
            } else if ((*s)[i] == '1') {
                bit_counts[i].one_count++;
            }
        }
    }

    part1(bit_counts);
    part2(binary_readings);
}

void part1(std::vector<BitCount>& bit_counts) {
    int gamma_rate = 0, epsilon_rate = 0;
    size_t i = 0;
    for (std::vector<BitCount>::reverse_iterator c = bit_counts.rbegin();
            c != bit_counts.rend(); c++, i++) {
        int max_bit = c->zero_count > c->one_count ? 0 : 1;
        int min_bit = c->zero_count < c->one_count ? 0 : 1;

        gamma_rate += (int) pow(2, i) * max_bit;
        epsilon_rate += (int) pow(2, i) * min_bit;
    }

    std::cout << gamma_rate * epsilon_rate << std::endl;
}

void part2(std::vector<std::string>& binary_readings) {
    // vector to hold BitCount of the remaining readings
    std::vector<BitCount> bit_counts (binary_readings[0].length());

    std::vector<std::string> remaining_readings (binary_readings.size());
    remaining_readings = binary_readings;

    size_t remaining_bits = bit_counts.capacity();

    while (remaining_bits--, remaining_readings.size() != 1) {
        // clear bit_counts and repopulate it with default in each iteration
        bit_counts.clear();
        for (size_t i = 0; i <  bit_counts.capacity(); i++) {
            bit_counts.push_back(BitCount { .zero_count = 0, .one_count = 0 });
        }
        
        // populate bit_counts for the remaining readings
        for (std::vector<std::string>::iterator reading = remaining_readings.begin();
                reading != remaining_readings.end(); reading++) {
            for (size_t i = 0; i < (*reading).size(); i++) {
                if ((*reading)[i] == '0') {
                    bit_counts[i].zero_count++;
                } else {
                    bit_counts[i].one_count++;
                }
            }
        }

        // filter remaining readings based of the bit_counts
        auto iter = std::remove_if (remaining_readings.begin(), remaining_readings.end(), [remaining_bits, bit_counts](std::string& reading) {
                    // current bit location of interest
                    size_t location = reading.length() - remaining_bits - 1;
                    
                    // return true if bit at current location is max bit for that location
                    return reading[location] == 
                    (bit_counts[location].zero_count >
                     bit_counts[location].one_count ? '0' : '1');
                });

        remaining_readings.erase(iter, remaining_readings.end());
        // std::cout << remaining_readings.size() << std::endl;
    }

    int btoi = 0;
    for (char bit : remaining_readings[0]) {
        int i = 0;
        btoi = (btoi * 2) + (bit == '0' ? 0 : 1) * int(pow(2, i++));
    }
    int co2_scrubber = btoi;

    std::cout << "CO2 Scrubber Rating: " << remaining_readings[0] << " " << btoi << std::endl;

    remaining_readings = binary_readings;
    remaining_bits = bit_counts.capacity();
    
    while (remaining_bits--, remaining_readings.size() != 1) {
        // clear bit_counts and repopulate it with default in each iteration
        bit_counts.clear();
        for (size_t i = 0; i <  bit_counts.capacity(); i++) {
            bit_counts.push_back(BitCount { .zero_count = 0, .one_count = 0 });
        }
        
        // populate bit_counts for the remaining readings
        for (std::vector<std::string>::iterator reading = remaining_readings.begin();
                reading != remaining_readings.end(); reading++) {
            for (size_t i = 0; i < (*reading).size(); i++) {
                if ((*reading)[i] == '0') {
                    bit_counts[i].zero_count++;
                } else {
                    bit_counts[i].one_count++;
                }
            }
        }

        // filter remaining readings based of the bit_counts
        auto iter = std::remove_if (remaining_readings.begin(), remaining_readings.end(), [remaining_bits, bit_counts](std::string& reading) {
                    // current bit location of interest
                    size_t location = reading.length() - remaining_bits - 1;
                    
                    // return true if bit at current location is min bit for that location
                    return reading[location] == 
                    (bit_counts[location].zero_count <=
                     bit_counts[location].one_count ? '0' : '1');
                });

        remaining_readings.erase(iter, remaining_readings.end());
        // std::cout << remaining_readings.size() << std::endl;
    }

    btoi = 0;
    for (char bit : remaining_readings[0]) {
        int i = 0;
        btoi = (btoi * 2) + (bit == '0' ? 0 : 1) * int(pow(2, i++));
    }
    int o2_generator = btoi;
    std::cout << "Oxygen Generator Rating: " << remaining_readings[0] << " " << btoi << std::endl;

    std::cout << co2_scrubber * o2_generator << std::endl;
}
