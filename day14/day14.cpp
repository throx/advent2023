#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "../shared/Point.h"

using namespace std;

typedef map<Point2, char> World;

int mx;
int my;

// Only need shuffle up, because it's easier to rotate the map
void shuffle_up(World& world) {
    for (int x = 0; x < mx; ++x) {
        int last = -1;
        for (int y = 0; y < my; ++y) {
            Point2 p({ x, y });
            auto it = world.find(p);
            if (it != world.end()) {
                char c = it->second;
                if (c == '#') {
                    last = y;
                }
                else if (c == 'O') {
                    world.erase(it);
                    ++last;
                    world[Point2({ x, last })] = 'O';
                }
            }
        }
    }
}

// Simple rotate function
World rotate(const World& world) {
    World ret;
    for (auto& e : world) {
        Point2 p({ my - e.first[1] - 1, e.first[0] });
        ret[p] = e.second;
    }
    return ret;
}

// Compute "value" of map
int value(const World& world) {
    int val = 0;
    for (auto& e : world) {
        if (e.second == 'O') {
            val += my - e.first[1];
        }
    }
    return val;
}

// For debugging
void print(const World& world) {
    for (int y = 0; y < my; ++y) {
        for (int x = 0; x < mx; ++x) {
            auto it = world.find(Point2({ x, y }));
            if (it == world.end()) {
                cout << '.';
            }
            else {
                cout << it->second;
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    World world;
    vector<int> lastblock;

    string s;
    int y = 0;
    while (getline(cin, s)) {
        for (int x = 0; x < s.length(); ++x) {
            char c = s[x];
            Point2 p({ x, y });
            if (c != '.') {
                world[p] = c;
            }
        }
        mx = s.length();
        ++y;
    }
    my = y;

    // First cycle, need to do it special because we have to extract part 1
    shuffle_up(world);
    int part1 = value(world);
    world = rotate(world);

    shuffle_up(world);
    world = rotate(world);

    shuffle_up(world);
    world = rotate(world);

    shuffle_up(world);
    world = rotate(world);

    int c = 1;

    // Store the world states after each cycle to look for a loop in positions
    map<World, int> bases;
    do {
        bases[world] = c;
        shuffle_up(world);
        world = rotate(world);
        shuffle_up(world);
        world = rotate(world);
        shuffle_up(world);
        world = rotate(world);
        shuffle_up(world);
        world = rotate(world);
        ++c;
    } while (bases.find(world) == bases.end());

    int period = c - bases[world];
    cout << "Found loop of period " << period << " at cycle " << c << endl;

    // Do cycles until a neat number of periods from a billion
    while ((1000000000 - c) % period != 0) {
        shuffle_up(world);
        world = rotate(world);
        shuffle_up(world);
        world = rotate(world);
        shuffle_up(world);
        world = rotate(world);
        shuffle_up(world);
        world = rotate(world);
        ++c;
    }

    // Grab the value
    int part2 = value(world);

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}