#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int main()
{
    string s;
    getline(cin, s);
    s = Split(s, ':')[1];
    vector<__int64> times = SplitInt64s(s, ' ');
    erase(s, ' ');
    __int64 time2 = stoll(s);

    getline(cin, s);
    s = Split(s, ':')[1];
    vector<__int64> cutoffs = SplitInt64s(s, ' ');
    erase(s, ' ');
    __int64 cutoff2 = stoll(s);

    __int64 part1 = 1;
    for (__int64 i = 0; i < times.size(); ++i) {
        __int64 ways = 0;
        for (__int64 hold = 0; hold < times[i]; ++hold) {
            __int64 dist = (times[i] - hold) * hold;
            if (dist > cutoffs[i]) {
                ++ways;
            }
        }
        part1 *= ways;
    }

    // BRUTE FORCE!  YAY!
    __int64 part2 = 0;
    for (__int64 hold = 0; hold < time2; ++hold) {
        __int64 dist = (time2 - hold) * hold;
        if (dist > cutoff2) {
            ++part2;
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}