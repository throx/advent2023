#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <set>
#include <list>
#include "../shared/Point.h"

using namespace std;
int mx, my;
map<Point2, int> loss;

void part1() {
    // Position, Direction, Direction count
    typedef tuple<Point2, Point2, int> State;

    // Curr, prev
    typedef tuple<State, State> StatePair;

    map<State, int> done;
    map<State, State> prev;
    multimap<int, StatePair> todo;

    Point2 dest = Point2({ mx - 1, my - 1 });
    int finalcost = 0;
    todo.insert({ 0, {{ Point2(), Point2(), 0 }, { Point2(), Point2(), 0 }} });
    State finalstate;

    while (!todo.empty()) {
        int cost = todo.begin()->first;
        State e = get<0>(todo.begin()->second);
        State prevstate = get<1>(todo.begin()->second);
        todo.erase(todo.begin());

        auto it = done.find(e);
        if (it != done.end()) {
            if (it->second <= cost) {
                continue;
            }
        }
        done[e] = cost;
        prev[e] = prevstate;

        Point2 pos = get<0>(e);
        Point2 dir = get<1>(e);
        int count = get<2>(e);

        if (pos == dest) {
            finalcost = cost;
            finalstate = e;
            break;
        }

        pos.DoSquareNeighbours([&](Point2 p) {
            if (p[0] < 0 || p[0] >= mx || p[1] < 0 || p[1] >= my) {
                return;
            }

            int newcount = 1;
            Point2 newdir = p - pos;
            if (newdir == dir) {
                newcount = count + 1;
                if (newcount > 3) {
                    return;
                }
            }
            if (newdir == -dir) {
                return;
            }

            int newcost = cost + loss.find(p)->second;
            State newstate({ p,  newdir, newcount });
            todo.insert({ newcost, {newstate, e} });
            });
    }

    set<Point2> finalpath;
    State ss = finalstate;
    while (get<0>(ss) != Point2()) {
        finalpath.insert(get<0>(ss));
        ss = prev[ss];
    }

    // RESULTS...
    for (int y = 0; y < my; ++y) {
        for (int x = 0; x < mx; ++x) {
            Point2 p({ x, y });
            if (finalpath.find(p) != finalpath.end()) {
                cout << loss.find(p)->second;
            }
            else {
                cout << ' ';
            }
        }
        cout << endl;
    }

    cout << "Part 1: " << finalcost << endl;

}

void part2() {
    // Position, Direction, Direction count
    typedef tuple<Point2, Point2, int> State;

    // Curr, prev
    typedef tuple<State, State> StatePair;

    map<State, int> done;
    map<State, State> prev;
    multimap<int, StatePair> todo;

    Point2 dest = Point2({ mx - 1, my - 1 });
    int finalcost = 0;
    todo.insert({ 0, {{ Point2(), Point2(), 0 }, { Point2(), Point2(), 0 }} });
    State finalstate;

    while (!todo.empty()) {
        int cost = todo.begin()->first;
        State e = get<0>(todo.begin()->second);
        State prevstate = get<1>(todo.begin()->second);
        todo.erase(todo.begin());

        auto it = done.find(e);
        if (it != done.end()) {
            if (it->second <= cost) {
                continue;
            }
        }
        done[e] = cost;
        prev[e] = prevstate;

        Point2 pos = get<0>(e);
        Point2 dir = get<1>(e);
        int count = get<2>(e);

        if (pos == dest) {
            if (count >= 4) {
                finalcost = cost;
                finalstate = e;
                break;
            }
            else {
                continue;
            }
        }

        pos.DoSquareNeighbours([&](Point2 p) {
            if (p[0] < 0 || p[0] >= mx || p[1] < 0 || p[1] >= my) {
                return;
            }

            int newcount = 1;
            Point2 newdir = p - pos;
            if (newdir == dir) {
                newcount = count + 1;
                if (newcount > 10) {
                    return;
                }
            }
            else if (newdir == -dir) {
                return;
            }
            else if (dir != Point2()) {
                if (count < 4) {
                    return;
                }
            }

            int newcost = cost + loss.find(p)->second;
            State newstate({ p,  newdir, newcount });
            todo.insert({ newcost, {newstate, e} });
            });
    }

    set<Point2> finalpath;
    State ss = finalstate;
    while (get<0>(ss) != Point2()) {
        finalpath.insert(get<0>(ss));
        ss = prev[ss];
    }

    // RESULTS...
    for (int y = 0; y < my; ++y) {
        for (int x = 0; x < mx; ++x) {
            Point2 p({ x, y });
            if (finalpath.find(p) != finalpath.end()) {
                cout << loss.find(p)->second;
            }
            else {
                cout << ' ';
            }
        }
        cout << endl;
    }

    cout << "Part 2: " << finalcost << endl;

}

int main()
{
    int y = 0;
    string s;
    while (getline(cin, s)) {
        for (int x = 0; x < s.length(); ++x) {
            Point2 p({ x, y });
            int i = s[x] - '0';
            loss[p] = i;
        }
        mx = s.length();
        ++y;
    }
    my = y;

    part1();
    part2();

    cout << "Press a key to continue" << endl;
    _getch();
}