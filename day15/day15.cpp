#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <list>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

unsigned char HASH(const string& step) {
    unsigned char h = 0;
    for (char c : step) {
        h = (h + (unsigned char)c) * 17;
    }
    return h;
}

int main()
{
    string s;
    getline(cin, s);
    vector<string> steps = Split(s, ',');

    int part1 = 0;

    typedef list<tuple<string, int>> Box;
    vector<Box> boxes;
    boxes.resize(256);

    for (auto step : steps) {

        // Add hashes
        part1 += HASH(step);

        // If a minus, find the label and remove it from the box list
        if (step.back() == '-') {
            string label = step.substr(0, step.length() - 1);
            int h = HASH(label);
            auto& box = boxes[h];
            box.remove_if([&](auto& x) {return get<0>(x) == label; });
        }

        // If an equals, find the label in the box and replace if it's there, otherwise add to back
        else {
            int power = step.back() - '0';
            string label = step.substr(0, step.length() - 2);
            int h = HASH(label);
            auto& box = boxes[h];
            auto it = find_if(box.begin(), box.end(), [&](auto& x) {return get<0>(x) == label; });
            if (it == box.end()) {
                box.push_back(make_tuple(label, power));
            }
            else {
                get<1>(*it) = power;
            }
        }
    }

    int part2 = 0;
    for (int b = 0; b < 256; ++b) {
        int slotnum = 1;
        for (auto& slot : boxes[b]) {
            part2 = part2 + (b + 1) * slotnum * get<1>(slot);
            ++slotnum;
        }
    }
    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}