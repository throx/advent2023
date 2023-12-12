#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <numeric>
#include <map>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("(\\S+) (\\S+)");

// Need to remember the state of the search so we can memoize the function (ie cache the answer
// for a particular state of computation) because it gets to the same spot a LOT
struct State {
    int pos;        // Position in the input
    int group;      // Group number we're currently checking
    int count;      // Current count of springs
    char next;      // What the next character is (because it's different from the constant input data)

    // C++ can't make its own less than operators (sigh)
    bool operator<(const State& other) const {
        if (pos > other.pos) return false;
        if (pos < other.pos) return true;
        if (group > other.group) return false;
        if (group < other.group) return true;
        if (count > other.count) return false;
        if (count < other.count) return true;
        if (next > other.next) return false;
        if (next < other.next) return true;
        return false;
    }
};

// Yeah, I should pass this as a parameter.
map<State, __int64> permcache;

__int64 perms_cached(string springs, const vector<int>& groups, State state);

// Uncached naive implementation - run through and recursively test each '?' with a spring or
// not a spring.  Maintain state through the recursion to make it easy to cache
__int64 perms_uncached(string springs, const vector<int>& groups, State state) {
    int group = state.group;
    int count = state.count;
    for (int x = state.pos; x < springs.length(); ++x) {
        char c = springs[x];
        switch (c) {
        case '.':
            if (count != 0) {
                if (groups[group] != count) {
                    return 0;
                }
                ++group;
                count = 0;
            }
            break;

        case '#':
            ++count;
            if (group == groups.size()) {
                return 0;
            }
            if (count > groups[group]) {
                return 0;
            }
            break;

        default:
            springs[x] = '.';
            __int64 c = perms_cached(springs, groups, State({ x, group, count, '.'}));
            springs[x] = '#';
            c += perms_cached(springs, groups, State({ x, group, count, '#' }));
            return c;
        }
    }

    if (count != 0) {
        if (groups[group] != count) {
            return 0;
        }
        ++group;
    }

    if (group != groups.size()) {
        return 0;
    }

    return 1;
}

// Caching function
__int64 perms_cached(string springs, const vector<int>& groups, State state) {
    auto it = permcache.find(state);
    if (it != permcache.end()) {
        return it->second;
    }

    __int64 p = perms_uncached(springs, groups, state);
    permcache[state] = p;
    return p;
}

// Wraps the caching function to reset the global cache and pass a constant state on entry
__int64 perms(string springs, const vector<int>& groups) {
    permcache.clear();
    return perms_uncached(springs, groups, State({0, 0, 0, 'x'}));
}

int main()
{
    __int64 part1 = 0;
    __int64 part2 = 0;
    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            vector<int> groups = SplitInts(sm[2]);
            __int64 p = perms(sm[1], groups);
            part1 += p;

            string s2 = sm[1].str() + "?" + sm[1].str() + "?" + sm[1].str() + "?" + sm[1].str() + "?" + sm[1].str();
            string p2 = sm[2].str() + "," + sm[2].str() + "," + sm[2].str() + "," + sm[2].str() + "," + sm[2].str();
            groups = SplitInts(p2);
            p = perms(s2, groups);
            part2 += p;
        }
    }


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}