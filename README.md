# Learning c++ with AdventOfCode-2021

## File Structure
for any day `n`(01-to-25)

day_n/
├── dayn.cpp
└── input.txt

Put your specific input in input.txt. Filename is used as cli input to the
executable.

## Compile and Run
It uses cmake so you need `cmake` installed and added to your path. 

Create a `build` directory at the root of this repository and run the following
commands from it:

```
$ cmake ..

$ cmake --build .
```

This will give you executables in build directory named in the format `dayn`
where `n` is the day number.

To run any particular days solution:

```shell
$ ./day03
usage: ./day01 <inputfile>

$ ./day03 ../day_03/input.txt
```
