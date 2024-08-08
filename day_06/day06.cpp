#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <array>

#define NDAYS_1 80 // number of simulation days for part 1
#define NDAYS_2 256 // number of simulation days for part 2
#define MAX_TIME 9 // maximum number of possible times for fishes

typedef size_t Time;
void simulate_day(std::vector<Time>& times);
void part1(std::vector<Time> times);
void part2(const std::vector<Time>& times);
void time_count_dump(const std::array<size_t, MAX_TIME>& time_counts);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <input-filename>" << std::endl;
        std::exit(-1);
    }

    std::string input_filename = argv[1];
    if (!std::filesystem::exists(input_filename)) {
        std::cerr << "error: file named '" << input_filename << "' does not exist"
            << std::endl;
        std::exit(-1);
    }

    std::ifstream input;
    input.open(input_filename);

    std::string line;
    std::getline(input, line);

    std::vector<Time> times;

    size_t comma_pos;
    while ((comma_pos = line.find(",")) != std::string::npos) {
        std::string time = line.substr(0, comma_pos);
        times.push_back(std::stoi(time));
        line = line.substr(comma_pos + 1, line.length());
    }

    if (line.length() != 0) { // process the last time
        times.push_back(std::stoi(line));
    }

    /*
    for (auto time : times) {
        std::cout << time << std::endl;
    }
    */

    part1(times);
    part2(times);
}

// Simulate a day for a vector of fish times.
void simulate_day(std::vector<Time>& times) {
    size_t new_fishes = 0; // number of new fishes that appear next day
    for (std::vector<Time>::iterator time = times.begin();
            time != times.end(); time++) {
        if (*time == 0) {
            new_fishes++;
            *time = 6;
        } else {
            *time -= 1;
        }
    }

    times.reserve(times.size() + new_fishes); // avoid unnecessary allocations
    for (size_t i = 0; i < new_fishes; i++) {
        times.push_back(8);
    }
}

void part1(std::vector<Time> times) {
    for (size_t i = 0; i < NDAYS_1; i++) {
        simulate_day(times);
    }

    std::cout << times.size() << std::endl;

    /*
    for (size_t i = 0; i < times.size(); i++) {
        std::cout << times[i] << (i == times.size() - 1 ? "\n" : ",");
    }
    */
}

void part2(const std::vector<Time>& times) {
    // An array to keep count of number of any particular time
    std::array<size_t, MAX_TIME> times_count;
    times_count.fill(0);
    for (std::vector<Time>::const_iterator time = times.begin();
            time != times.end(); time++) {
        times_count[*time]++;
    }

    for (size_t day = 0; day < NDAYS_2; day++) {
        size_t zero_count = times_count[0]; // save number of fishes with 0 days
        for (std::array<size_t, MAX_TIME>::iterator count = times_count.begin();
                count != times_count.end(); count++) {
            if (count != times_count.end() - 1) {
                *count = *(count + 1);
            } else {
                *count = zero_count;
            }

        }
        times_count[6] += zero_count;
    }

    size_t total_fishes = 0;
    for (auto count : times_count) {
        total_fishes += count;
    }

    std::cout << total_fishes << std::endl;
}

// print time count of different fish times possible
void time_count_dump(const std::array<size_t, MAX_TIME>& time_counts) {
    for (size_t i = 0; i < MAX_TIME; i++) {
        std::printf("%4lu%c", time_counts[i], i == MAX_TIME - 1 ? '\n' : ',');
    }
}
