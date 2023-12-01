#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>

using namespace std;

const regex FIRST_R("(\\d).*");
const regex LAST_R(".*(\\d)");
const regex FIRST2_R("(\\d|one|two|three|four|five|six|seven|eight|nine).*");
const regex LAST2_R(".*(\\d|one|two|three|four|five|six|seven|eight|nine)");

map<string, int> MAPPING = {
    {"1", 1},
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

int main()
{
    int part1 = 0;
    int part2 = 0;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        int first = 0;
        int last = 0;
        if (regex_search(s.cbegin(), s.cend(), sm, FIRST_R)) {
            first = stoi(sm[1]);
        }
        if (regex_search(s.cbegin(), s.cend(), sm, LAST_R)) {
            last = stoi(sm[1]);
        }

        part1 += 10 * first + last;

        if (regex_search(s.cbegin(), s.cend(), sm, FIRST2_R)) {
            first = MAPPING[sm[1]];
        }
        if (regex_search(s.cbegin(), s.cend(), sm, LAST2_R)) {
            last = MAPPING[sm[1]];
        }

        part2 += 10 * first + last;
    }


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}