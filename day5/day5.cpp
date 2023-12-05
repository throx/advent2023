#include <map>
#include <set>
#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "../shared/Split.h"

using namespace std;

struct Triple {
    __int64 dst;
    __int64 src;
    __int64 count;

    Triple(const vector<__int64>& v) {
        dst = v[0];
        src = v[1];
        count = v[2];
    }
};

int main()
{
    string s;
    getline(cin, s);        // Seeds
    s = s.substr(7);
    vector<__int64> seeds = SplitInt64s(s, ' ');

    vector<map<__int64, Triple>> maps(7);

    cin >> s;   // blank line

    for (int i = 0; i < 7; ++i) {
        getline(cin, s);   // header
        getline(cin, s);   // first map line
        while (!s.empty() && !cin.eof()) {
            Triple t(SplitInt64s(s, ' '));

            // We store these according to their END point (well, end+1 actually) in the map, because that's
            // the way C++ maps work nicely with the upper_bound function.  Basically, it returns the element
            // in the map that is bounded on the high side by the argument, so if you store the end of the
            // sections as the index then the one you're looking for is just upper_bound of the input arg.
            maps[i].insert({ t.src + t.count, t });
            getline(cin, s);   // next line
        }
        __int64 first = maps[i].begin()->second.src;
        if (first != 0) {
            maps[i].insert({ first, Triple({0, 0, first}) });
        }
    }

    // Fold maps together
    map<__int64, Triple> themap = maps[0];
    for (int i = 1; i < 7; ++i) {

        // Create a reverse mapping
        map<__int64, Triple> revmap;
        for (auto& e : themap) {
            revmap.insert({ e.second.dst + e.second.count, e.second });
        }

        // Get all the breakpoints for analysis (0, starts, ends) from themap
        //  and (0, starts, ends) from maps[i] REVERSE MAPPED through revmap
        set<__int64> starts;
        starts.insert(0);
        for (auto& e : themap) {
            starts.insert(e.second.src);    // Start
            starts.insert(e.first);         // End
        }
        for (auto& e : maps[i]) {
            __int64 src = e.second.src;
            auto it = revmap.upper_bound(src);
            if (it != revmap.end()) {
                Triple& t = it->second;
                src = src - (t.dst - t.src);
            }
            starts.insert(src);

            __int64 end = e.first;
            it = revmap.upper_bound(end);
            if (it != revmap.end()) {
                Triple& t = it->second;
                end = end - (t.dst - t.src);
            }
            starts.insert(end);
        }

        // Loop through break points and analyse what the combo is at each
        map<__int64, Triple> nextmap;
        auto it = starts.begin();
        __int64 src = *it;
        while ((++it) != starts.end()) {
            __int64 count = *it - src;
            __int64 diff = 0;

            auto it1 = themap.upper_bound(src);
            if (it1 != themap.end()) {
                Triple& t = it1->second;
                diff = t.dst - t.src;
            }

            __int64 src2 = src + diff;
            auto it2 = maps[i].upper_bound(src2);
            if (it2 != maps[i].end()) {
                Triple& t = it2->second;
                diff += t.dst - t.src;
            }

            Triple newt({ src + diff, src, count });
            nextmap.insert({ src + count, newt });
            src = *it;
        }

        themap = nextmap;
    }

    // Find value of each seed
    __int64 min_location = INT64_MAX;
    for (__int64 seed : seeds) {
        __int64 loc = seed;

        // My original code for part 1, now folded into THEMAP
        //cout << seed;
        //for (int i = 0; i < 7; ++i) {
        //    auto it = maps[i].upper_bound(loc);
        //    if (it != maps[i].end()) {
        //        Triple& t = it->second;
        //        if (loc >= t.src) {
        //            loc = t.dst + (loc - t.src);
        //        }
        //    }
        //    cout << " -> " << loc;
        //}

        //cout << endl;

        auto it = themap.upper_bound(loc);
        if (it != themap.end()) {
            Triple& t = it->second;
            loc = loc + (t.dst - t.src);
        }

        if (loc < min_location) {
            min_location = loc;
        }
    }

    // So, for part 2, the critical points are the start of each seed range and each break point in the map
    // that's in a seed range.  Look at them all and see what has a smallest output
    set<__int64> breaks;
    auto seed_it = seeds.begin();
    while (seed_it != seeds.end()) {
        __int64 start = *seed_it;
        ++seed_it;
        __int64 end = start + *seed_it;
        ++seed_it;

        breaks.insert(start);

        // Inefficient.  Should do something with lower_bound and upper_bound.  Oh well.
        for (auto& e : themap) {
            if (e.second.src >= start && e.second.src < end) {
                breaks.insert(e.second.src);
            }
        }
    }

    // Now loop the breaks and find smallest mapping
    __int64 min_location2 = INT64_MAX;
    for (__int64 seed : breaks) {
        __int64 loc = seed;

        auto it = themap.upper_bound(loc);
        if (it != themap.end()) {
            Triple& t = it->second;
            loc = loc + (t.dst - t.src);
        }

        if (loc < min_location2) {
            min_location2 = loc;
        }
    }


    cout << "Part 1: " << min_location << endl;
    cout << "Part 2: " << min_location2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}