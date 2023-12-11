#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <set>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int main()
{
    set<Point2> galaxies;
    set<int> seenx;
    set<int> seeny;

    // Read in data

    int mx, my;
    int y = 0;
    string s;
    while (getline(cin, s)) {
        int x = 0;
        for (char c : s) {
            Point2 p({ x, y });
            if (c == '#') {
                galaxies.insert(p);
                seenx.insert(x);
                seeny.insert(y);
            }
            ++x;
        }
        mx = x;
        ++y;
    }
    my = y;

    // Compute expansion
    vector<int> expandx;
    int nx = 0;
    for (int x = 0; x < mx; ++x) {
        expandx.push_back(nx);
        if (seenx.find(x) == seenx.end()) {
            ++nx;
        }
    }

    vector<int> expandy;
    int ny = 0;
    for (y = 0; y < my; ++y) {
        expandy.push_back(ny);
        if (seeny.find(y) == seeny.end()) {
            ++ny;
        }
    }

    // Do expansion
    set<Point2> expgal;
    for (auto& p : galaxies) {
        expgal.insert(p + Point2({ expandx[p[0]], expandy[p[1]] }));
    }

    // Shortest paths (actually delta x + delta y)
    int part1 = 0;
    for (auto it1 = expgal.begin(); it1 != expgal.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        while (it2 != expgal.end()) {
            part1 += Dist(*it1, *it2);    
            ++it2;
        }
    }

    // Do expansion 2 (just 999999 times expansion 1)
    expgal.clear();
    for (auto& p : galaxies) {
        expgal.insert(p + Point2({ expandx[p[0]], expandy[p[1]] }) * 999999);
    }

    // Shortest paths (actually delta x + delta y)
    __int64 part2 = 0;
    for (auto it1 = expgal.begin(); it1 != expgal.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        while (it2 != expgal.end()) {
            part2 += Dist(*it1, *it2);
            ++it2;
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}