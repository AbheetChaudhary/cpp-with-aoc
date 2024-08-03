#include <fstream>
#include <iostream>
#include <vector>

struct Command {
    char direction; // f, u or d
    int amount;
};

void part1(std::vector<Command> &);
void part2(std::vector<Command> &);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " " << "<inputfile>" << std::endl;
        std::exit(-1);
    }
    char *filename = argv[1];
    std::ifstream input;
    input.open(filename);

    std::string line;

    // vector to hold commands
    std::vector<Command> commands;

    // parse and populate commands
    while (std::getline(input, line)) {
        if (line.size() == 0) continue;
        if (line.find("up") != std::string::npos) {
            commands.push_back(Command { .direction = 'u', 
                    .amount = std::stoi(line.substr(3)) });
        } else if (line.find("down") != std::string::npos) {
            commands.push_back(Command { .direction = 'd', 
                    .amount = std::stoi(line.substr(5)) });
        } else if (line.find("forward") != std::string::npos) {
            commands.push_back(Command { .direction = 'f', 
                    .amount = std::stoi(line.substr(8)) });
        }
    }

    /*
    for (size_t i = 0; i < commands.size(); i++) {
        std::cout << commands[i].direction << ", " << commands[i].amount
            << std::endl;
    }
    */

    part1(commands);
    part2(commands);
}

void part1(std::vector<Command> &commands) {
    int depth = 0, horizontal = 0;
    for (std::vector<Command>::iterator c = commands.begin();
            c != commands.end(); c++) {
        if (c->direction == 'u') depth -= c->amount;
        else if (c->direction == 'd') depth += c->amount;
        else horizontal += c->amount;
    }

    /*
    std::cout << "depth: " << depth << std::endl;
    std::cout << "horizontal: " << horizontal << std::endl;
    */

    std::cout << depth * horizontal << std::endl;
}

void part2(std::vector<Command> &commands) {
    int depth = 0, horizontal = 0, aim = 0;
    for (std::vector<Command>::iterator c = commands.begin();
            c != commands.end(); c++) {
        if (c->direction == 'u') {
            aim -= c->amount;
        }
        else if ((*c).direction == 'd') {
            aim += c->amount;
        }
        else {
            horizontal += c->amount;
            depth += aim * c->amount;
        }
    }

    /*
    std::cout << "depth: " << depth << std::endl;
    std::cout << "horizontal: " << horizontal << std::endl;
    */

    std::cout << depth * horizontal << std::endl;
}
