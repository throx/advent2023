#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <set>
#include <numeric>
#include "../shared/Split.h"
#include "../shared/Point.h"
#include "../shared/Rect.h"

using namespace std;
vector<map<Point2, int>> blocks;
vector<Rect3> index;

const regex INPUT_R("(\\d+),(\\d+),(\\d+)~(\\d+),(\\d+),(\\d+)");

set<int> get_above(int id) {
    const Rect3& block = index[id];

    set<int> above;
    int z_test = block.end()[2];
    for (int x = block.start()[0]; x < block.end()[0]; ++x) {
        for (int y = block.start()[1]; y < block.end()[1]; ++y) {
            Point2 p({ x, y });
            auto it = blocks[z_test].find(p);
            if (it != blocks[z_test].end()) {
                above.insert(it->second);
                cout << it->second << " is above " << id << endl;
            }
        }
    }

    return above;
}

set<int> get_below(int id) {
    const Rect3& block = index[id];

    set<int> below;
    int z_test = block.start()[2] - 1;
    for (int x = block.start()[0]; x < block.end()[0]; ++x) {
        for (int y = block.start()[1]; y < block.end()[1]; ++y) {
            Point2 p({ x, y });
            auto it = blocks[z_test].find(p);
            if (it != blocks[z_test].end()) {
                below.insert(it->second);
                cout << it->second << " is below " << id << endl;
            }
        }
    }

    return below;
}

void erase_block(int id) {
    const Rect3& r = index[id];

    for (int x = r.start()[0]; x < r.end()[0]; ++x) {
        for (int y = r.start()[1]; y < r.end()[1]; ++y) {
            Point2 p({ x, y });

            for (int z = r.start()[2]; z < r.end()[2]; ++z) {

                if (blocks[z][p] != id) {
                    throw "BOOM";
                }
                blocks[z].erase(p);
            }
        }
    }
    index[id] = Rect3();
}

bool is_stable_with(const Rect3 new_block) {
    if (new_block.start()[2] == 1) {
        return true;
    }

    int z_test = new_block.start()[2] - 1;
    for (int x = new_block.start()[0]; x < new_block.end()[0]; ++x) {
        for (int y = new_block.start()[1]; y < new_block.end()[1]; ++y) {
            Point2 p({ x, y });
            if (blocks[z_test].find(p) != blocks[z_test].end()) {
                return true;
            }
        }
    }
    return false;
}

void fall(int id) {
    Rect3 r = index[id];
    erase_block(id);

    while (true) {
        if (is_stable_with(r)) {

            for (int x = r.start()[0]; x < r.end()[0]; ++x) {
                for (int y = r.start()[1]; y < r.end()[1]; ++y) {
                    Point2 p({ x, y });

                    for (int z = r.start()[2]; z < r.end()[2]; ++z) {

                        if (blocks[z].find(p) != blocks[z].end()) {
                            throw "BOOM";
                        }

                        blocks[z][p] = id;
                    }
                }
            }
            ++id;
            break;
        }

        r = Rect3(r.start() - P3::Z, r.end() - P3::Z);
    }
}

int main()
{
    int id = 0;
    blocks.resize(1000);
    string s;
    map<int, set<Rect3>> inblocks;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            Point3 p1({ stoi(sm[1]),stoi(sm[2]), stoi(sm[3]) });
            Point3 p2({ stoi(sm[4]),stoi(sm[5]), stoi(sm[6]) });
            if ((p1[0] > p2[0]) || (p1[1] > p2[1]) || (p1[2] > p2[2])) {
                throw "BOOM";
            }

            p2 += Point3({ 1,1,1 });
            Rect3 r(p1, p2);
            inblocks[p1[2]].insert(r);
        }
    }

    // Have to make them fall by z-order
    for (auto& e : inblocks) {
        for (auto r : e.second) {
            while (true) {
                if (is_stable_with(r)) {
                    
                    index.push_back(r);
                    for (int x = r.start()[0]; x < r.end()[0]; ++x) {
                        for (int y = r.start()[1]; y < r.end()[1]; ++y) {
                            Point2 p({ x, y });

                            for (int z = r.start()[2]; z < r.end()[2]; ++z) {

                                if (blocks[z].find(p) != blocks[z].end()) {
                                    throw "BOOM";
                                }

                                blocks[z][p] = id;
                            }
                        }
                    }
                    ++id;
                    cout << "Block " << id << " at " << r << endl;
                    break;
                }

                r = Rect3(r.start() - P3::Z, r.end() - P3::Z);
            }
        }
    }

    set<int> unsafe;

    for (int i = 0; i < index.size(); ++i) {
        set<int> below = get_below(i);
        if (below.size() == 1) {
            unsafe.insert(*below.begin());
        }
        if (below.size() == 0) {
            auto r = index[i];
            if (r.start()[2] != 1) {
                throw "BOOM";
            }
        }
    }

    int part1 = index.size() - unsafe.size();

    int part2 = 0;
    for (int i = 0; i < index.size(); ++i) {
        cout << "Falling " << i << endl;
        auto blocks_save = blocks;
        auto index_save = index;

        set<int> to_fall = get_above(i);
        erase_block(i);

        while (!to_fall.empty()) {
            auto it = min_element(to_fall.begin(), to_fall.end(), [](int r1, int r2) { return index[r1].start()[2] < index[r2].start()[2]; });
            int id = *it;
            to_fall.erase(it);

            if (get_below(id).size() == 0) {
                cout << "  " << id << " fell" << endl;
                set<int> new_fall = get_above(id);
                to_fall.insert(new_fall.begin(), new_fall.end());
                fall(id);
                ++part2;
            }
        }

        index = index_save;
        blocks = blocks_save;
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}