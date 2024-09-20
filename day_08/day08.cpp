#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <set>
#include <cassert>
#include <algorithm>
#include <array>

using std::cout;
using std::endl;
using std::cerr;

/*
   1 -> 2 (uniq)
   2 -> 5
   3 -> 5
   4 -> 4 (uniq)
   5 -> 5
   6 -> 6
   7 -> 3 (uniq)
   8 -> 7 (uniq)
   9 -> 6
   0 -> 6
*/

struct Reading {
    std::vector<std::set<char>> patterns; // input patterns
    std::vector<std::set<char>> outputs; // output patterns
};

// Create Reading struct by taking input and output patterns seperated by ' '
// and both seperated by '|' delimiter.
Reading create(std::string& pattern_string,
        std::string& output_string) {

    std::vector<std::set<char>> patterns, outputs;
    patterns.reserve(10);
    outputs.reserve(4);

    // Create reading.patterns vector.
    char* loc;
    loc = std::strtok(pattern_string.data(), " ");
    std::set<char> set;
    for (auto c : std::string(loc)) {
        set.insert(c);
    }
    patterns.push_back(set);
    while ((loc = std::strtok(nullptr, " "))) {
        // Reallocation happens here.
        std::set<char> set;
        for (auto c : std::string(loc)) {
            set.insert(c);
        }
        patterns.push_back(set);
    }

    // Create reading.outputs vector.
    loc = std::strtok(output_string.data(), " ");
    set.clear();
    for (auto c : std::string(loc)) {
        set.insert(c);
    }
    outputs.push_back(set);
    while ((loc = std::strtok(nullptr, " "))) {
        // Reallocation happens here.
        std::set<char> set;
        for (auto c : std::string(loc)) {
            set.insert(c);
        }
        outputs.push_back(set);
    }

    assert(patterns.size() == 10 && "Incorrect input patterns count");
    assert(outputs.size() == 4 && "Incorrect output patterns count");

    return Reading {
        .patterns = patterns,
        .outputs = outputs,
    };
}

void part1(const std::vector<struct Reading>& readings);
void part2(const std::vector<struct Reading>& readings);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " <input-filename>" << endl;
        std::exit(-1);
    }

    if (!std::filesystem::exists(argv[1])) {
        cerr << "error: could not find file named '" << argv[1] << "'" 
            << endl;
        std::exit(-1);
    }

    std::string input_filename = argv[1];
    std::ifstream inputfile;
    inputfile.open(input_filename);

    std::string line;
    std::vector<struct Reading> readings;

    // Each entry must be on a single line.
    while (std::getline(inputfile, line)) {
        size_t pipe_location = line.find("|");
        std::string patterns = line.substr(0, pipe_location);
        std::string outputs = line.substr(pipe_location + 1, line.size());
        readings.push_back(create(patterns, outputs));
    }

    part1(readings);
    part2(readings);
}

void
part1(const std::vector<struct Reading>& readings) {
    // Number of times either 1, 4, 7 or 8 appear in output.
    int count = 0;

    for (auto& reading : readings) {
        for (auto& output : reading.outputs) {
            size_t size = output.size();
            if (size == 2 || size == 3 || size == 4 || size == 7) {
                count++;
            }
        }
    }

    cout << count << endl;
}

/*
 *  pppp
 * q    r
 * q    r
 *  ssss
 * t    u
 * t    u
 *  vvvv
 *
 * Known:
 * 1 - ru
 * 4 - qsru
 * 7 - pru
 * 8 - pqrstu
 *
 * length = 5:
 * 2 - prstv
 * 3 - prsuv
 * 5 - pqsuv
 *
 * length = 6:
 * 6 - pqstuv
 * 9 - pqrsuv
 * 0 - pqrtuv
 *
 * qs -> in 4 but not in 1
 *
 * 3 -> length 5 and contains 1
 * 0 -> length 6 but does not contain qs
 * 9 -> length 6 and contains 4
 * 6 -> length 6 and not equal to 0, 9
 *
 * 5 -> length 5 and contained in 6
 * 2 -> length 5 and not equal to 3, 5
 *
 */

// Finds the set that is the difference of a bigger and smaller set.
// Bigger and smaller order matters.
std::set<char>
set_difference(const std::set<char>& bigger,
        const std::set<char>& smaller) {
    assert(bigger.size() >= smaller.size() && "First argument is not bigger");

    std::set<char> diff;

    for (auto c_big : bigger) {
        bool in_smaller = false;
        for (auto c_small : smaller) {
            if (c_big == c_small) {
                in_smaller = true;
                break;
            }
        }
        if (!in_smaller) {
            diff.insert(c_big);
        }
    }

    return diff;
}

// Returns true if bigger set contains the smaller set.
// Bigger and smaller order matters.
// Returns true if smaller set is empty.
bool
set_contains(const std::set<char>& bigger,
        const std::set<char>& smaller) {

    assert(bigger.size() >= smaller.size() && "First argument is not bigger");

    for (auto c_small : smaller) {
        bool is_contained = false;
        for (auto c_big : bigger) {
            if (c_big == c_small) {
                is_contained = true;
                break;
            }
        }

        if (!is_contained) {
            return false;
        }
    }

    return true;
}

// Pretty print a set of char.
void
set_debug_dump(const std::set<char>& set) {
    for (auto c : set) {
        cout << c << " ";
    }
    cout << endl;
}

// Decodes the input pattern and converts output pattern to int.
int
decoded(const Reading& reading) {
    // Ordered sets for different 1, 4, 7, 8
    std::vector<std::set<char>> set_n;
    set_n.reserve(4);

    // sizes of uniquely known digits.
    size_t sizes[] = {2 /* 1 */, 4 /* 4 */, 3 /* 7 */, 7 /* 8 */};

    for (int i = 0; i < 4; i++) {
        // vector to hold results of filtering reading.patterns
        std::vector<std::set<char>> temp;

        std::copy_if(reading.patterns.begin(), reading.patterns.end(), 
                std::back_inserter(temp), [i, sizes](std::set<char> set) {
                    return set.size() == sizes[i];
                });
        assert(temp.size() ==  1);
        set_n.push_back(temp[0]);
    }

    std::set<char> set_one, set_four, set_seven, set_eight;
    // Known sets for numbers 1, 4, 7 and 8.
    set_one = set_n[0];
    set_four = set_n[1];
    set_seven = set_n[2];
    set_eight = set_n[3];

    std::set<char> set_q_s = set_difference(set_four, set_one);
    assert(set_q_s.size() == 2 && "Could not find set_q_s");

    // Sets with length 5 and 6.
    std::vector<std::set<char>> penta_sets, hexa_sets;

    for (auto& set : reading.patterns) {
        if (set.size() == 5) {
            penta_sets.push_back(set);
        } else if (set.size() == 6) {
            hexa_sets.push_back(set);
        }
    }

    // Set of length 5 that contains set_one.
    std::vector<std::set<char>> set_contains_set_one;
    std::copy_if(penta_sets.begin(),
            penta_sets.end(),
            std::back_inserter(set_contains_set_one),
            [set_one](std::set<char>& set) {
                return set_contains(set, set_one);
            });

    assert(set_contains_set_one.size() == 1 && "Could not find set_three");
    std::set<char> set_three = set_contains_set_one[0];


    // Set with length 6 but does not contain set_q_s
    std::vector<std::set<char>> set_not_contain_q_s;
    std::copy_if(hexa_sets.begin(),
            hexa_sets.end(),
            std::back_inserter(set_not_contain_q_s),
            [set_q_s](std::set<char>& set) {
                return !set_contains(set, set_q_s);
            });

    assert(set_not_contain_q_s.size() == 1 && "Could not find set_zero");
    std::set<char> set_zero = set_not_contain_q_s[0];

    // Set length 6 but contains set_four
    std::vector<std::set<char>> set_contains_set_four;
    std::copy_if(hexa_sets.begin(),
            hexa_sets.end(),
            std::back_inserter(set_contains_set_four),
            [set_four](std::set<char>& set) {
                return set_contains(set, set_four);
            });

    assert(set_contains_set_four.size() == 1 
            && "Could not find set_nine");
    std::set<char> set_nine = set_contains_set_four[0];

    // Set with length 6 that is not set_zero and set_nine.
    std::set<char> set_six;
    for (auto& set : hexa_sets) {
        if (set != set_zero && set != set_nine) {
            set_six = set;
        }
    }
    assert(set_six.size() == 6 && "Could not find set_six");

    // Set set_five, contained in set_six
    std::set<char> set_five;
    for (auto& set : penta_sets) {
        if (set_contains(set_six, set)) {
            set_five = set;
        }
    }
    assert(set_five.size() == 5 && "Could not find set_five");

    // Set set_two, length five and not equal to set_three and set_five.
    std::set<char> set_two;
    for (auto& set : penta_sets) {
        if (set != set_five && set != set_three) {
            set_two = set;
        }
    }
    assert(set_two.size() == 5 && "Could not find set_two");

    // Sets corresponding to all numbers are known at this point.
    std::array<std::set<char>, 10> mappings = {
        set_zero,
        set_one,
        set_two,
        set_three,
        set_four,
        set_five,
        set_six,
        set_seven,
        set_eight,
        set_nine,
    };

    // Convert reading.output to decimal number.
    int num = 0;
    for (auto& output : reading.outputs) {
        for (int i = 0; i < 10; i++) {
            if (output == mappings[i]) {
                num = num * 10 + i;
                break;
            }
        }
    }

    return num;
}

void
part2(const std::vector<struct Reading>& readings) {
    int sum = 0;
    // Decode and sum.
    for (auto& reading : readings) {
        sum += decoded(reading);
    }
    cout << sum << endl;
}
