#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <numeric>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("(\\S+) (\\S+)");

int xx;

int perms(char* springs, int len, const vector<int>& groups) {
    int group = 0;
    int count = 0;
    int left = accumulate(groups.begin(), groups.end(), 0);
    for (int x = 0; x < len; ++x) {
        char c = springs[x];
        switch (c) {
        case '.':
            if (count != 0) {
                if (groups[group] != count) {
                    return 0;
                }
                left -= count;
                ++group;
                count = 0;

                if (left + groups.size() - group > len - x) {
                    return 0;
                }
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
            int c = perms(springs, len, groups);
            springs[x] = '#';
            c += perms(springs, len, groups);
            springs[x] = '?';
            while (x < xx) {
                cout << '.';
                --xx;
            }
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

int perms(const string& str, const vector<int>& groups) {
    char* s = new char[str.length()];
    memcpy(s, str.data(), str.length());
    xx = str.length();
    return perms(s, xx, groups);
    delete s;
}

int main()
{
    int part1 = 0;
    int part2 = 0;
    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            vector<int> groups = SplitInts(sm[2]);
            int p = perms(sm[1], groups);
            part1 += p;

            string s2 = sm[1].str() + "?" + sm[1].str() + "?" + sm[1].str() + "?" + sm[1].str() + "?" + sm[1].str();
            string p2 = sm[2].str() + "," + sm[2].str() + "," + sm[2].str() + "," + sm[2].str() + "," + sm[2].str();
            cout << endl << s2 << " = " << endl;
            groups = SplitInts(p2);
            p = perms(s2, groups);
            cout << p << endl;
            part2 += p;
        }
    }


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}