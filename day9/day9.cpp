#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "../shared/Split.h"
#include <algorithm>
#include <list>

using namespace std;

int main()
{
    // The whole problem is linear, so we'll just work with the sum as a single
    // vector, rather than compute all the intermediate bits.
    vector<int> all;
    string s;
    while (getline(cin, s)) {
        vector<int> line = SplitInts(s, ' ');
        all.resize(line.size(), 0);
        transform(all.begin(), all.end(), line.begin(), all.begin(), plus<int>());
    }

    // Compute the differences iteratively.  We only care about the last difference in each "line"
    // for part 1 and the first difference in each "line" for part 2, so store them in "last" and "first"
    // vectors respectively.
    vector<int> subs = all;
    list<int> last;
    list<int> first;
    bool zero = false;
    while (!zero) {
        auto it = subs.begin();
        auto it2 = it;
        ++it2;
        zero = true;
        int diff;
        int first_diff = INT_MAX;

        while (it2 != subs.end()) {
            diff = *it2 - *it;
            *it = diff;
            zero = zero && (diff == 0);
            ++it;
            ++it2;

            if (first_diff == INT_MAX) {
                first_diff = diff;
            }
        }

        if (!zero) {
            last.push_back(diff);
            first.push_back(first_diff);
        }

        subs.resize(subs.size() - 1);
    }

    // The next in the series is just the previous value plus the sum of all the last differences
    __int64 part1 = *all.rbegin();
    for (int l : last) {
        part1 += l;
    }

    // The previous in the series is just the first value plus the alternation (+/-/+/- etc.) of
    // the first differences in the set.
    __int64 part2 = *all.begin();
    __int64 m = -1;
    for (int f : first) {
        part2 += (m * f);
        m = -m;
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}