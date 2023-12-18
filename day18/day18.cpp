#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <set>
#include "../shared/Point.h"
#include "../shared/Rect.h"

using namespace std;

const regex INPUT_R("(.) (\\d+) \\(#(.....)(.)\\)");
typedef set<tuple<int, int>> Line;
typedef map<int, Line> World;

__int64 fill(const World& world, const Rect2& bound)
{
    __int64 ret = 0;
    for (int y = bound.start()[1]; y < bound.end()[1]; ++y) {
        auto& line = world.find(y)->second;
        bool paint = false;
        int px = 0;
        for (auto e : line) {
            int x = get<0>(e);
            int turn = get<1>(e);

            if (paint) {
                ret += x - px;
            }
            else {
                ++ret;
            }

            if (turn >= 0) {
                paint = !paint;
            }
            
            px = x;
        }
    }

    return ret;
}

int main()
{
    World world1;
    Point2 pos1; 
    Point2 first1, prev1;
    Rect2 bound1;

    World world2;
    Point2 pos2;
    Point2 first2, prev2;
    Rect2 bound2;

    // Record the edges in the world map so that the floodfill can just do alternate painting
    // Edges are computed as corners, where the cross product is stored for determining if the
    // corner leads inside or outside the space, or as plain edges where the painting simply
    // alternates.

    // I just copy/pasted for part 2 rather than define a function.  Bad me.

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            Point2 dir = P2::DIRS[sm[1].str()[0]];
            int count = stoi(sm[2]);

            if (first1 == Point2()) {
                first1 = dir;
            }
            if (prev1 != Point2()) {
                world1[pos1[1]].insert({ pos1[0], P2::Cross(prev1, dir) });
            }
            prev1 = dir;

            if (dir[0] == 0) {
                pos1 += dir;
                for (int i = 1; i < count; ++i) {
                    world1[pos1[1]].insert({pos1[0], 0});
                    pos1 += dir;
                }
            }
            else {
                pos1 += (dir * count);
            }
            bound1 = bound1.ExpandTo(pos1);

            dir = P2::DIRS["RDLU"s[stoi(sm[4])]];
            count = stoi(sm[3], nullptr, 16);
            if (first2 == Point2()) {
                first2 = dir;
            }
            if (prev2 != Point2()) {
                world2[pos2[1]].insert({ pos2[0], P2::Cross(prev2, dir) });
            }
            prev2 = dir;

            if (dir[0] == 0) {
                pos2 += dir;
                for (int i = 1; i < count; ++i) {
                    world2[pos2[1]].insert({ pos2[0], 0 });
                    pos2 += dir;
                }
            }
            else {
                pos2 += (dir * count);
            }
            bound2 = bound2.ExpandTo(pos2);
        }
    }
    world1[0].insert({ 0, P2::Cross(prev1, first1) });
    world2[0].insert({ 0, P2::Cross(prev2, first2) });

    // RESULTS...

    __int64 part1 = fill(world1, bound1);
    cout << "Part 1: " << part1 << endl;

    __int64 part2 = fill(world2, bound2);
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}