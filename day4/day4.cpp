#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <numeric>
#include <map>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("Card\\s+(\\d+):\\s*(.*)\\s\\|\\s(.*)");

int main()
{
    int part1 = 0;

    map<int, int> cardcount;

    int lines = 0;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            ++lines;
            int card = stoi(sm[1]);
            vector<int> wins = SplitInts(sm[2], ' ');
            vector<int> mynums = SplitInts(sm[3], ' ');

            sort(wins.begin(), wins.end());
            sort(mynums.begin(), mynums.end());

            vector<int> mywins;
            set_intersection(wins.begin(), wins.end(), mynums.begin(), mynums.end(), back_inserter(mywins));

            if (!mywins.empty()) {
                part1 = part1 + (1 << (mywins.size() - 1));
            }

            int mycopies = 1 + cardcount[card];
            cardcount[card] = mycopies;
            for (int c = 1; c <= mywins.size(); ++c) {
                cardcount[card + c] += mycopies;
            }            
        }
    }

    int part2 = 0;
    for (int i = 1; i <= lines; ++i) {
        part2 += cardcount[i];
    }


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}