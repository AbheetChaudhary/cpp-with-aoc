#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <assert.h>

// a point with x and y coordinates that can have a danger level based on the
// number of points there
struct Point {
    int x;
    int y;

    bool operator==(const struct Point& p) const {
        return this->x == p.x && this->y == p.y;
    }

    bool operator!=(const struct Point& p) const {
        return this->x != p.x || this->y != p.y;
    }
};

// hash function for a point
namespace std {
    template <>
    struct hash<Point> {
        size_t operator() (const Point& p) const {
            size_t hash_x = std::hash<int>{}(p.x);
            size_t hash_y = std::hash<int>{}(p.y);
            
            return hash_x ^ (hash_y << 1);
        }
    };
}

struct Line { // line of points
    // begin is not necessarily a point that comes before the end on a plane,
    // end_1 and end_2 would have been a better name.
    struct Point begin;
    struct Point end;
};

// four spacial arrangements a line can have
enum Direction {
    invalid, singleton, horizontal, vertical, diagonal, reverse_diagonal
};

// Line with spatial arrangement information
struct DirectedLine {
    struct Point begin;
    struct Point end;
    Direction direction;
};

void point_dump(const struct Point& point);
void part1(const std::vector<Line>& vent_lines);
void part2(const std::vector<Line>& vent_lines);
Direction get_direction(const Line& line);
void increment_current_point(const DirectedLine& line, Point& current_point);

int main(int argc, char* argv[]) {
    // check if a cli argument has been given
    if (argc != 2) {
        std::printf("usage: %s <input-filename>\n", argv[0]);
        std::exit(-1);
    }

    // Check if file exists. Exit if not
    std::string input_filename = argv[1];
    if (!std::filesystem::exists(input_filename)) {
        std::printf("error: file '%s' does not exist\n", input_filename.c_str());
        std::exit(-1);
    }

    std::ifstream inputfile;
    inputfile.open(input_filename);

    std::vector<Line> vent_lines;

    std::string line;
    while (std::getline(inputfile, line)) {
        std::string arrow_delimiter = "->";
        size_t arrow_location = line.find(arrow_delimiter);

        // point 1
        std::string point_string = line.substr(0, arrow_location);
        std::string x_coordinate_string, y_coordinate_string;
        size_t comma_location = point_string.find(",");
        x_coordinate_string = point_string.substr(0, comma_location);
        y_coordinate_string = point_string.substr(comma_location + 1 /* comma length */,
                point_string.length());
        struct Point begin = Point {
            .x = std::stoi(x_coordinate_string),
            .y = std::stoi(y_coordinate_string),
        };

        // point 2
        point_string = line.substr(arrow_location +
                arrow_delimiter.length(), line.length());
        comma_location = point_string.find(",");
        x_coordinate_string = point_string.substr(0, comma_location);
        y_coordinate_string = point_string.substr(comma_location + 1 /* comma length */,
                point_string.length());
        struct Point end = Point {
            .x = std::stoi(x_coordinate_string),
            .y = std::stoi(y_coordinate_string),
        };

        vent_lines.push_back(Line { .begin = begin, .end = end });
    }

    part1(vent_lines);
    part2(vent_lines);

}

// pretty print a Point
void point_dump(const struct Point& point) {
    std::cout << "(x: " << point.x << ", y: " << point.y << ")";
}

void part1(const std::vector<Line>& vent_lines) {
    // unordered map of all points with non zero danger level
    std::unordered_map<Point, int> danger_map;

    for (std::vector<Line>::const_iterator line = vent_lines.begin();
            line != vent_lines.end(); line++) {
        if (line->begin.x != line->end.x && line->begin.y != line->end.y) {
            // not a straight line
            continue;
        }

        // check horizontal or vertical line
        bool is_horizontal = false;
        if (line->begin.y == line->end.y) {
            is_horizontal = true;
        }

        // Current point while iterating through a line of vents.
        // Not a true iterator, just a hack.
        struct Point current_point = Point {
            .x = line->begin.x,
            .y = line->begin.y,
        };

        for (;;) {
            // check if point is in danger_points
            if (danger_map.find(current_point) == danger_map.end()) { // first time at this point
                                                                      // initialize it
                std::pair<Point, int> new_point (current_point, 1);
                danger_map.insert(new_point); // insert with danger = 1
            } else {
                // point already seen, increment danger
                std::unordered_map<Point, int>::iterator p = danger_map.find(current_point);
                if(p != danger_map.end())
                    (p->second)++;
            }

            // exit loop if at end point
            if (current_point.x == line->end.x && current_point.y == line->end.y) {
                break;
            }

            // go to next point
            if (is_horizontal) {
                if (line->begin.x < line->end.x) {
                    current_point.x++;
                } else {
                    current_point.x--;
                }
            } else {
                if (line->begin.y < line->end.y) {
                    current_point.y++;
                } else {
                    current_point.y--;
                }
            }

        }
    }

    int high_danger_points = 0;
    for (std::unordered_map<Point, int>::iterator p = danger_map.begin();
            p != danger_map.end(); p++) {
        if (p->second > 1) {
            high_danger_points++;
        }
    }

    std::cout << high_danger_points << std::endl;
}

void part2(const std::vector<Line>& vent_lines) {
    std::vector<DirectedLine> directed_vent_lines;
    directed_vent_lines.reserve(vent_lines.size());

    for (std::vector<Line>::const_iterator l = vent_lines.begin();
            l != vent_lines.end(); l++) {
        directed_vent_lines.push_back({l->begin, l->end, get_direction(*l)});
    }

    std::unordered_map<Point, int> danger_map;

    for (std::vector<DirectedLine>::const_iterator directed_line = directed_vent_lines.begin();
            directed_line != directed_vent_lines.end(); directed_line++) {

        // invalid direction found
        if (directed_line->direction == invalid) {
            continue;
        }

        struct Point current_point = {directed_line->begin.x, directed_line->begin.y};

        for (;;) {
            if (danger_map.find(current_point) == danger_map.end()) {
                // seeing the point for the first time, insert it in 
                // danger map and initialize danger level to 1
                danger_map.insert(std::pair<Point, int> (current_point, 1));
            } else {
                // point already exist in danger_map, increment danger by one
                ((danger_map.find(current_point))->second)++;
            }

            // reached the line end
            if (current_point == directed_line->end) {
                break;
            }

            increment_current_point(*directed_line, current_point);
        }
    }

    int high_danger_points = 0;
    for (std::unordered_map<Point, int>::const_iterator danger_point = danger_map.begin();
            danger_point != danger_map.end(); danger_point++) {
        if (danger_point->second > 1) {
            high_danger_points++;
        }
    }
    
    std::cout << high_danger_points << std::endl;
}

// get direction from a line
Direction get_direction(const Line& line) {
    struct Point begin = line.begin;
    struct Point end = line.end;

    if (begin == end) {
        return singleton;
    } else if (begin.x == end.x) {
        return vertical;
    } else if (begin.y == end.y) {
        return horizontal;
    } else if (end.y - begin.y == end.x - begin.x) {
        return diagonal;
    } else if(end.y - begin.y == -1 * (end.x - begin.x)) {
      return reverse_diagonal;
    } else {
        return invalid;
    }
}

/*
 * Takes a line and a current point on that line and increments the 
 * current point to point to the next point based on the direction of 
 * the line. 
 *
 * Line must not be a singleton point
 */
void increment_current_point(const DirectedLine& line, Point& current_point) {
    struct Point begin = line.begin;
    struct Point end = line.end;

    assert(line.direction != singleton);

    if (line.direction == horizontal) {
        if (begin.x < end.x) {
            (current_point.x)++;
        } else {
            (current_point.x)--;
        }
    } else if (line.direction == vertical) {
        if (begin.y < end.y) {
            (current_point.y)++;
        } else {
            (current_point.y)--;
        }
    } else if (line.direction == diagonal) {
        if (begin.x < end.x) {
            (current_point.x)++;
            (current_point.y)++;
        } else {
            (current_point.x)--;
            (current_point.y)--;
        }
    } else {
        if (begin.x < end.x) {
            (current_point.x)++;
            (current_point.y)--;
        } else {
            (current_point.x)--;
            (current_point.y)++;
        }
    }
}
