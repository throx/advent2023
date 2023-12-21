#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <set>
#include <numeric>
#include "../shared/Point.h"
#include "../shared/Rect.h"

using namespace std;

Rect2 bounds;
set<Point2> rocks;
typedef map<Point2, int> Steps;

// Returns an always positive modulo.  Good for infinite gardens
//   https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
inline int modulo_Euclidean(int a, int b) {
    int m = a % b;
    if (m < 0) {
        m = (b < 0) ? m - b : m + b;
    }
    return m;
}

int main()
{
    int y = 0;
    Point2 start;

    string s;
    while (getline(cin, s)) {
        for (int x = 0; x < s.length(); ++x) {
            Point2 p({ x, y });
            bounds = bounds.ExpandTo(p);
            if (s[x] == 'S') {
                start = p;
            }
            else if (s[x] == '#') {
                rocks.insert(p);
            }
        }
        ++y;
    }

    int width = bounds.end()[0];

    // Simple floodfill
    Steps steps;
    set<Point2> todo;
    int step = 0;
    todo.insert(start);

    while (!todo.empty()) {
        set<Point2> todonext;
        for (Point2 p : todo) {
            if (rocks.find(p) == rocks.end()) {
                steps[p] = step;
                p.DoSquareNeighbours([&](auto& n) {
                    if (bounds.Contains(n) && steps.find(n) == steps.end()) {
                        todonext.insert(n);
                    }
                });
            }
        }
        ++step;
        todo.swap(todonext);
    }

    int part1 = count_if(steps.begin(), steps.end(), [](auto& e) { return e.second % 2 == 0 && e.second <= 64; });

    cout << "Part 1: " << part1 << endl;

    // Reset everything for part 2
    todo.clear();
    steps.clear();
    step = 0;
    todo.insert(start);

    // Hard code maximum steps
    int maxstep = 26501365;

    // We know that the result will trend quadratic, and will likely repeat a pattern based on the width of the
    // garden.  Create a set of double-differences to detect when the quadratic stabilises on a garden-width base
    vector<__int64> counts;
    counts.resize(width);
    vector<__int64> diffs;
    diffs.resize(width);
    vector<__int64> ddiffs;
    ddiffs.resize(width);

    // Floodfill with modulos to find rocks (repeating garden)
    while (step <= maxstep) {
        set<Point2> todonext;
        for (Point2 p : todo) {
            Point2 rock_p({ modulo_Euclidean(p[0], width), modulo_Euclidean(p[1], width) });
            if (rocks.find(rock_p) == rocks.end()) {
                steps[p] = step;
                p.DoSquareNeighbours([&](auto& n) {
                    if (steps.find(n) == steps.end()) {
                        todonext.insert(n);
                    }
                    });
            }
        }

        // Find out the current differences from the previous (width) gardens ago
        int modstep = step % width;
        int count = count_if(steps.begin(), steps.end(), [&](auto& e) { return e.second % 2 == step % 2; });
        int diff = count - counts[modstep];
        ddiffs[modstep] = diff - diffs[modstep];
        diffs[modstep] = diff;
        counts[modstep] = count;

        // Every (width) iterations, check if the double-differences have stabilised.  When they have, break because
        // we can predict everything from here.
        if (modstep == 0) {
            int ddiff = ddiffs[0];
            bool same = true;
            for (int i = 1; i < width; ++i) {
                if (ddiff != ddiffs[i]) {
                    same = false;
                    break;
                }
            }
            if (same) {
                break;
            }
        }
        ++step;
        todo.swap(todonext);
    }

    cout << "Pattern found at step " << step << endl;

    // Yeah, I could compute it.  It's only a few million iterations though and this avoided any more thinking
    // because my head hurt.
    int modstep;
    for (++step; step <= maxstep; ++step) {
        modstep = step % width;
        diffs[modstep] += ddiffs[0];
        counts[modstep] += diffs[modstep];
    }

    __int64 part2 = counts[modstep];    

    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}