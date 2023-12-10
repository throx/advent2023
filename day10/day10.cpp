#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int main()
{
    map<Point2, char> pipes;
    map<Point2, char> loop;

    int mx = 0;
    int my = 0;

    // Read the input into a map of point -> what's there.
    int y = 0;
    string line;
    Point2 start;
    while (getline(cin, line)) {
        mx = line.length();
        for (int x = 0; x < line.length(); ++x) {
            Point2 pos({ x, y });
            char c = line[x];
            pipes[pos] = c;
            if (c == 'S') {
                start = pos;
            }
        }
        ++y;
    }
    my = y;

    // Interpret start
    bool n = "7F|"s.find(pipes[start + P2::N]) != string::npos;
    bool s = "LJ|"s.find(pipes[start + P2::S]) != string::npos;
    bool e = "7J-"s.find(pipes[start + P2::E]) != string::npos;
    bool w = "FL-"s.find(pipes[start + P2::W]) != string::npos;
    if (n && s) pipes[start] = '|';
    if (e && w) pipes[start] = '-';
    if (n && e) pipes[start] = 'L';
    if (n && w) pipes[start] = 'J';
    if (s && e) pipes[start] = 'F';
    if (s && w) pipes[start] = '7';

    // Trace the loop around.  Part 1 is just half the length of the loop
    Point2 pos = start;
    int dist = 0;
    do {
        // Save off the main loop for part 2
        loop[pos] = pipes[pos];
        ++dist;
        if (n) {
            pos += P2::N;
            n = "LJ|"s.find(pipes[pos]) != string::npos;
            s = false;
            e = "FL-"s.find(pipes[pos]) != string::npos;
            w = "7J-"s.find(pipes[pos]) != string::npos;
        }
        else if (s) {
            pos += P2::S;
            n = false;
            s = "7F|"s.find(pipes[pos]) != string::npos;
            e = "FL-"s.find(pipes[pos]) != string::npos;
            w = "7J-"s.find(pipes[pos]) != string::npos;
        }
        else if (e) {
            pos += P2::E;
            n = "LJ|"s.find(pipes[pos]) != string::npos;
            s = "7F|"s.find(pipes[pos]) != string::npos;
            e = "FL-"s.find(pipes[pos]) != string::npos;
            w = false;
        }
        else if (w) {
            pos += P2::W;
            n = "LJ|"s.find(pipes[pos]) != string::npos;
            s = "7F|"s.find(pipes[pos]) != string::npos;
            e = false;
            w = "7J-"s.find(pipes[pos]) != string::npos;
        }
    } while (pos != start);
    int part1 = dist / 2;

    // Finding which tile is just a state machine.  Start off each row on the "outside".
    // Each vertical pipe you cross switches you to "inside" and back.
    // Each corner will switch you to a state where inside is up or down.  You can then
    // figure out from the next corner whether you are in or out by the way the corner goes.
    // If you hit a space, add up the "inside" area if your state is "inside".
    // (I left my map printing code in just because it's pretty)
    int part2 = 0;
    for (int y = 0; y < my; ++y) {
        enum { STATE_OUT, STATE_IN, STATE_UP, STATE_DOWN } state = STATE_OUT;
        for (int x = 0; x < mx; ++x) {
            Point2 pos = Point2({ x, y });
            if (loop.find(pos) == loop.end()) {
                if (state == STATE_OUT) {
                    loop[pos] = '.';
                }
                else if (state == STATE_IN) {
                    loop[pos] = 'X';
                    ++part2;
                }
                else {
                    throw "boom";
                }
            }
            else {
                switch (loop[pos]) {
                case '|' :
                    switch (state) {
                    case STATE_OUT:
                        state = STATE_IN;
                        break;
                    case STATE_IN:
                        state = STATE_OUT;
                        break;
                    default:
                        throw "bang";
                    }
                    break;
                case '-' :
                    if (state == STATE_IN || state == STATE_OUT) {
                        throw "bang";
                    }
                    break;
                case 'L' :
                    switch (state) {
                    case STATE_OUT:
                        state = STATE_UP;
                        break;
                    case STATE_IN:
                        state = STATE_DOWN;
                        break;
                    default:
                        throw "bang";
                    }
                    break;
                case 'F':
                    switch (state) {
                    case STATE_OUT:
                        state = STATE_DOWN;
                        break;
                    case STATE_IN:
                        state = STATE_UP;
                        break;
                    default:
                        throw "bang";
                    }
                    break;
                case 'J':
                    switch (state) {
                    case STATE_UP:
                        state = STATE_OUT;
                        break;
                    case STATE_DOWN:
                        state = STATE_IN;
                        break;
                    default:
                        throw "bang";
                    }
                    break;
                case '7':
                    switch (state) {
                    case STATE_UP:
                        state = STATE_IN;
                        break;
                    case STATE_DOWN:
                        state = STATE_OUT;
                        break;
                    default:
                        throw "bang";
                    }
                    break;
                }
            }
            cout << loop[pos];
        }
        cout << endl;
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}