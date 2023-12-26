#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <set>
#include <list>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("(\\w{3}): (.*)");

typedef map<string, set<string>> World;
typedef map<pair<string, string>, int> NumPaths;

string farthest(const World& world, const string& start) {
    multimap<int, string> todo;
    set<string> done;
    string ret;
    todo.insert({ 0, start });
    while (!todo.empty()) {
        auto it = todo.begin();
        int steps = it->first;
        string where = it->second;
        todo.erase(it);

        if (done.find(where) == done.end()) {
            done.insert(where);
            for (string next : world.at(where)) {
                todo.insert({ steps + 1, next });
            }
            ret = where;
        }
    }

    return ret;
}

list<string> path(const World& world, const string& start, const string& to) {
    multimap<int, pair<string, list<string>>> todo;
    map<string, int> done;
    list<string> startpath;
    startpath.push_back(start);
    todo.insert({ 0, {start, startpath} });
    while (!todo.empty()) {
        auto it = todo.begin();
        int steps = it->first;
        string where = it->second.first;
        list<string> path = it->second.second;
        todo.erase(it);

        if (where == to) {
            return path;
        }

        if (done.find(where) == done.end()) {
            done[where] = steps;
            for (string next : world.at(where)) {
                path.push_back(next);
                todo.insert({ steps + 1, {next, path} });
                path.pop_back();
            }
        }
    }
    return list<string>();
}

int connected(const World& world, const string& start) {
    set<string> done;
    set<string> todo;
    todo.insert(start);
    while (!todo.empty()) {
        string curr = *todo.begin();
        todo.erase(todo.begin());

        if (done.find(curr) == done.end()) {
            done.insert(curr);

            for (string next : world.at(curr)) {
                todo.insert(next);
            }
        }
    }
    return done.size();
}

int main()
{
    World world;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            for (string targ : Split(sm[2], ' ')) {
                world[sm[1]].insert(targ);
                world[targ].insert(sm[1]);
            }
        }
    }

    // Find max dist
    string p1 = farthest(world, world.begin()->first);
    string p2 = farthest(world, p1);

    // Bridge must be on shortest paths.  Knock out items and compute new paths, repeat 3 times and see if we have a disjoint set.
    list<string> path1 = path(world, p1, p2);
    World snipped = world;
    auto it = path1.begin();
    string prev1 = *it;
    ++it;
    while (it != path1.end()) {
        string next1 = *it;
        snipped[prev1].erase(next1);
        snipped[next1].erase(prev1);

        list<string> path2 = path(snipped, p1, p2);
        World snipped2 = snipped;
        auto it2 = path2.begin();
        string prev2 = *it2;
        ++it2;
        while (it2 != path2.end()) {
            string next2 = *it2;
            snipped2[prev2].erase(next2);
            snipped2[next2].erase(prev2);

            list<string> path3 = path(snipped2, p1, p2);
            World snipped3 = snipped2;
            auto it3 = path3.begin();
            string prev3 = *it3;
            ++it3;
            while (it3 != path3.end()) {
                string next3 = *it3;
                snipped3[prev3].erase(next3);
                snipped3[next3].erase(prev3);

                //cout << "Try: " << prev1 << ":" << next1 << ", " << prev2 << ":" << next2 << ", " << prev3 << ":" << next3 << endl;
                list<string> path4 = path(snipped3, p1, p2);
                if (path4.empty()) {
                    int part1 = connected(snipped3, p1) * connected(snipped3, p2);
                    cout << "Part1: " << part1 << endl;
                    goto end;
                }

                snipped3 = snipped2;
                ++it3;
                prev3 = next3;
            }

            snipped2 = snipped;
            ++it2;
            prev2 = next2;
        }

        snipped = world;
        ++it;
        prev1 = next1;
    }

    end:
    // RESULTS...
    cout << "Press a key to continue" << endl;
    _getch();
}