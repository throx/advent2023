#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <conio.h>

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");
vector<string> sch;
int part1 = 0;

void process_num(int start, int end, int y)
{
    int num = stoi(sch[y].substr(start, end - start));

    bool is_part = false;
    for (int yy = y - 1; yy <= y + 1; ++yy) {
        if (yy < 0 || yy >= sch.size()) continue;
        for (int xx = start - 1; xx < end + 1; ++xx) {
            if (xx < 0 || xx >= sch[yy].length()) continue;
            char ch = sch[yy][xx];
            if (!isdigit(ch) && ch != '.') {
                is_part = true;
            }
        }
    }

    if (is_part) {
        part1 += num;
    }
}

int get_a_num(vector<string>& sch_temp, int x, int y) {
    for (int yy = y - 1; yy <= y + 1; ++yy) {
        if (yy < 0 || yy >= sch_temp.size()) continue;
        for (int xx = x - 1; xx <= x + 1; ++xx) {
            if (xx < 0 || xx >= sch_temp[yy].length()) continue;
            char ch = sch_temp[yy][xx];
            if (isdigit(ch)) {
                
                // Find first num in sequence
                while (xx > 0 && isdigit(sch_temp[yy][xx - 1])) {
                    --xx;
                }

                int start = xx;

                // Find last num in sequence
                while (xx < sch_temp[yy].length() && isdigit(sch_temp[yy][xx])) {
                    ++xx;
                }

                int num = stoi(sch_temp[yy].substr(start, xx - start));

                // Blank it out
                for (int x2 = start; x2 < xx; ++x2) {
                    sch_temp[yy][x2] = ' ';
                }
                return num;
            }
        }
    }
    return 0;
}

int main()
{

    while (!cin.eof()) {
        string s;
        cin >> s;
        sch.push_back(s);
    }

    // PART 1
    for (int y = 0; y < sch.size(); ++y) {
        int digit_start = -1;
        for (int x = 0; x < sch[y].length(); ++x) {
            if (digit_start == -1) {
                if (isdigit(sch[y][x])) {
                    digit_start = x;
                }
            }
            else {
                if (!isdigit(sch[y][x])) {
                    process_num(digit_start, x, y);
                    digit_start = -1;
                }
            }
        }
        if (digit_start != -1) {
            process_num(digit_start, sch[y].length(), y);
        }
    }

    // PART 2
    int part2 = 0;
    for (int y = 0; y < sch.size(); ++y) {
        for (int x = 0; x < sch[y].length(); ++x) {
            if (sch[y][x] == '*') {
                // Have a cog.  Find the digits around it
                auto sch_temp = sch;
                int num1 = get_a_num(sch_temp, x, y);
                int num2 = get_a_num(sch_temp, x, y);
                int num3 = get_a_num(sch_temp, x, y);
                if (num1 == 0 || num2 == 0 || num3 != 0) continue;
                part2 += num1 * num2;
            }
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}