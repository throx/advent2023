#include <iostream>
#include <string>
#include <regex>
#include <conio.h>

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int main()
{
    while (!cin.eof()) {
        string s;
        cin >> s;

        // DO stuff
    }

    // OR...

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            // Store data here
        }
    }


    // RESULTS...

    cout << "Part 1: " << 0 << endl;
    cout << "Part 2: " << 0 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}