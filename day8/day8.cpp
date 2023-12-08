#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <numeric>
#include <conio.h>

using namespace std;

const regex INPUT_R("(...) = \\((...), (...)\\)");

struct Node {
    string left;
    string right;
};

int main()
{
    string seq;
    getline(cin, seq);

    string s;
    getline(cin, s);

    map<string, Node> nodes;

    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            nodes[sm[1]] = Node({ sm[2], sm[3] });
        }
    }

    int part1 = 0;
    int p = 0;
    string pos = "AAA";

    // Skip for the part 2 test
    if (nodes.find("AAA") != nodes.end()) {
        while (pos != "ZZZ") {
            if (seq[p] == 'L') {
                pos = nodes[pos].left;
            }
            else {
                pos = nodes[pos].right;
            }

            ++part1;
            ++p;
            if (p == seq.length()) {
                p = 0;
            }
        }
    }

    __int64 part2 = 1;

    for (auto& e : nodes) {
        if (e.first[2] == 'A') {
            pos = e.first;
            int p = 0;
            __int64 count = 0;
            while (pos[2] != 'Z') {
                if (seq[p] == 'L') {
                    pos = nodes[pos].left;
                }
                else {
                    pos = nodes[pos].right;
                }

                ++count;
                ++p;
                if (p == seq.length()) {
                    p = 0;
                }
            }

            cout << e.first << " -> " << pos << " = " << count << endl;

            // Love C++17 having std::lcm
            part2 = lcm(part2, count);
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}