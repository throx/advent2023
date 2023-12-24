#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include "../shared/Point.h"
#include "day23.h"

using namespace std;

int dfs(const map<int, map<int, int>>& edges, int from = 0, int currdist = 0, vector<int> donenodes = vector<int>())
{
    donenodes.push_back(from);
    if (from == 1) {
        return currdist;
    }

    int best = 0;
    for (auto& edge : edges.at(from)) {
        if (find(donenodes.begin(), donenodes.end(), edge.first) == donenodes.end()) {
            int dist = dfs(edges, edge.first, currdist + edge.second, donenodes);

            if (dist > best) {
                best = dist;
            }
        }
    }
    return best;
}

int main()
{
    map<Point2, char> world;
    int mx = 0;
    int my = 0;

    string s;
    int y = 0;
    while (getline(cin, s)) {
        for (int x = 0; x < s.length(); ++x) {
            Point2 p({ x, y });
            world[p] = s[x];
        }
        mx = s.length();
        ++y;
    }
    my = y;

    Point2 start = Point2({ 1,0 });
    Point2 finish = Point2({ mx - 2, my - 1 });

    // Find nodes
    vector<Point2> nodes;
    map<Point2, int> nodeindex;

    nodeindex[start] = 0;
    nodes.push_back(start);
    nodeindex[finish] = 1;
    nodes.push_back(finish);

    for (int y = 0; y < my; ++y) {
        for (int x = 0; x < mx; ++x) {
            int count = 0;
            Point2 p({ x, y });
            p.DoSquareNeighbours([&](auto& n) {
                auto it = world.find(n);
                if (it == world.end()) return;
                if (it->second != '#') ++count;
            });
            if (count > 2) {
                nodeindex[p] = nodes.size();
                nodes.push_back(p);
            }
        }
    }

    // Find edges
    map<int, map<int, int>> edges;  // from -> (to -> distance)
    for (int node = 0; node < nodes.size(); ++node) {
        multimap<int, Point2> todo;
        set<Point2> done;
        todo.insert({ 0, nodes[node] });
        while (!todo.empty()) {
            auto it = todo.begin();
            Point2 p = it->second;
            int dist = it->first;
            todo.erase(it);
            done.insert(p);

            auto it2 = nodeindex.find(p);
            if (it2 != nodeindex.end() && it2->second != node) {
                edges[node][it2->second] = dist;
                continue;
            }

            p.DoSquareNeighbours([&](auto& n) {
                if (done.find(n) != done.end()) return;

                auto it3 = world.find(n);
                if (it3 == world.end()) return;

                char c = it3->second;
                if (c == '#') return;

                if (c == '.') {
                    todo.insert({ dist + 1, n });
                    return;
                }

                Point2 dir = n - p;
                if (c == 'v' && dir != P2::S) return;
                if (c == '<' && dir != P2::W) return;
                if (c == '>' && dir != P2::E) return;
                if (c == '^' && dir != P2::N) return;

                todo.insert({ dist + 1, n });
            });

        }
    }

    // DFS
    int part1 = dfs(edges);

    // Part 2 edges change
    edges.clear();  // from -> (to -> distance)
    for (int node = 0; node < nodes.size(); ++node) {
        multimap<int, Point2> todo;
        set<Point2> done;
        todo.insert({ 0, nodes[node] });
        while (!todo.empty()) {
            auto it = todo.begin();
            Point2 p = it->second;
            int dist = it->first;
            todo.erase(it);
            done.insert(p);

            auto it2 = nodeindex.find(p);
            if (it2 != nodeindex.end() && it2->second != node) {
                edges[node][it2->second] = dist;
                continue;
            }

            p.DoSquareNeighbours([&](auto& n) {
                if (done.find(n) != done.end()) return;

                auto it3 = world.find(n);
                if (it3 == world.end()) return;

                char c = it3->second;
                if (c == '#') return;

                todo.insert({ dist + 1, n });
            });
        }
    }

    int part2 = dfs(edges);

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}