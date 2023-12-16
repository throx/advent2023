#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int mx, my;

int beam(const map<Point2, char>& world, Point2 pos, Point2 dir) {
    set<tuple<Point2, Point2>> done, doing;
    set<Point2> energy;

    doing.insert({ pos, dir });

    while (!doing.empty()) {
        auto e = *doing.begin();
        doing.erase(doing.begin());

        pos = get<0>(e);
        dir = get<1>(e);

        if (done.find({ pos, dir }) != done.end()) {
            continue;
        }
        done.insert({ pos, dir });

        pos += dir;
        if (pos[0] < 0 || pos[0] >= mx || pos[1] < 0 || pos[1] >= my) {
            continue;
        }

        energy.insert(pos);

        auto it = world.find(pos);
        if (it == world.end()) {
            doing.insert({ pos, dir });
            continue;
        }

        char c = it->second;
        switch (c) {
        case '/':
            dir = Point2({ -dir[1], -dir[0] });
            doing.insert({ pos, dir });
            break;

        case '\\':
            dir = Point2({ dir[1], dir[0] });
            doing.insert({ pos, dir });
            break;

        case '|':
            if (dir[0] == 0) {
                doing.insert({ pos, dir });
            }
            else {
                doing.insert({ pos, P2::U });
                doing.insert({ pos, P2::D });
            }
            break;

        case '-':
            if (dir[1] == 0) {
                doing.insert({ pos, dir });
            }
            else {
                doing.insert({ pos, P2::L });
                doing.insert({ pos, P2::R });
            }
            break;
        }
    }

    return energy.size();
}

int main()
{
    map<Point2, char> world;

    string s;
    int y = 0;
    while (getline(cin, s)) {
        for (int x = 0; x < s.length(); ++x) {
            char c = s[x];
            if (c != '.') {
                Point2 p({ x, y });
                world[p] = c;
            }
        }
        mx = s.length();
        ++y;
    }
    my = y;

    // RESULTS...
    int part1 = beam(world, Point2{ (-1, 0) }, P2::R);

    int part2 = 0;
    for (int i = 0; i < mx; ++i) {
        part2 = max(part2, beam(world, Point2({ i, -1 }), P2::D));
        part2 = max(part2, beam(world, Point2({ i, my }), P2::U));
    }
    for (int i = 0; i < my; ++i) {
        part2 = max(part2, beam(world, Point2({ -1, i }), P2::R));
        part2 = max(part2, beam(world, Point2({ mx, i }), P2::L));
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}