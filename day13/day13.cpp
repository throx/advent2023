#include <iostream>
#include <string>
#include <regex>
#include <conio.h>

using namespace std;

vector<string> transpose(const vector<string>& s)
{
    vector<string> ret;
    for (int x = 0; x < s[0].length(); ++x) {
        string a;
        for (int y = 0; y < s.size(); ++y) {
            a.push_back(s[y][x]);
        }
        ret.push_back(a);
    }
    return ret;
}

int findreflect(const vector<string>& s)
{
    // Find two the same
    for (int i = 0; i < s.size() - 1; ++i) {
        // Test to ends
        int j = i;
        int k = i + 1;
        bool match = true;
        while (j >= 0 && k < s.size()) {
            if (s[j] != s[k]) {
                match = false;
                break;
            }
            --j;
            ++k;
        }
        if (match) {
            return i + 1;
        }
    }
    return 0;
}

int stringdiff(const string& s1, const string& s2) {
    int diff = 0;
    for (int i = 0; i < s1.length(); ++i) {
        if (s1[i] != s2[i]) {
            ++diff;
        }
    }
    return diff;
}

int findreflect2(const vector<string>& s)
{
    // Find two the same-ish
    for (int i = 0; i < s.size() - 1; ++i) {
        // Test to ends
        int j = i;
        int k = i + 1;
        int diff = 0;
        while (j >= 0 && k < s.size()) {
            diff += stringdiff(s[j], s[k]);
            if (diff > 1) {
                break;
            }
            --j;
            ++k;
        }
        if (diff == 1) {
            return i + 1;
        }
    }
    return 0;
}

int main()
{
    int part1 = 0;
    int part2 = 0;
    vector<string> terrain;

    string s;
    while (getline(cin, s)) {
        if (s.empty()) {
            part1 += 100 * findreflect(terrain) + findreflect(transpose(terrain));
            part2 += 100 * findreflect2(terrain) + findreflect2(transpose(terrain));
            terrain.clear();
        }
        else {
            terrain.push_back(s);
        }
    }

    if (!terrain.empty()) {
        part1 += 100 * findreflect(terrain) + findreflect(transpose(terrain));
        part2 += 100 * findreflect2(terrain) + findreflect2(transpose(terrain));
    }


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}