#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <conio.h>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("Game (\\d+): (.*)");
map<string, int> MAX_MAP = {
    {"red", 12},
    {"green", 13},
    {"blue", 14}
};

int main()
{
    int part1 = 0;
    int part2 = 0;
    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            int game_id = stoi(sm[1]);
            bool ok = true;
            map<string, int> max_found;
            for (string game : Split(sm[2], ';')) {
                for (string cube : Split(game, ',')) {
                    auto c = Split(trim(cube), ' ');
                    int num = stoi(c[0]);
                    string colour = c[1];
                    if (num > MAX_MAP[colour]) {
                        ok = false;
                    }
                    max_found[colour] = max(max_found[colour], num);
                }
            }
            if (ok) {
                part1 += game_id;
            }
            part2 += max_found["red"] * max_found["blue"] * max_found["green"];
        }
    }


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}