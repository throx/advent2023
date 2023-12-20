#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <set>
#include <list>
#include <numeric>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("(.+) -> (.+)");

struct Component {
    char type;
    vector<string> outputs;
    set<string> inputs;

    bool ison;  // Flipflop
    set<string> highinputs; // Conjunction

    Component() :
        type('x'),
        ison(false)
    {}

    Component(char t, vector<string> out) : 
        type(t), 
        outputs(out),
        ison(false)
    {}
};

struct Pulse {
    bool ishigh;
    string component;
    string source;
};

int main()
{
    map<string, Component> components;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            string component = sm[1];
            vector<string> outputs = Split(sm[2], ',');
            char type = 'b';
            if (component != "broadcaster") {
                type = component[0];
                component = component.substr(1);
            }
            components[component] = Component(type, outputs);
        }
    }

    // Map inputs
    for (auto& c : components) {
        for (auto& o : c.second.outputs) {
            components[o].inputs.insert(c.first);
        }
    }

    int lowsum = 0;
    int highsum = 0;
    int turn = 0;

    // For me, zh is the input to rx, and tracing the graph the result is the LCM of the inputs turning low
    map<string, __int64> zhturns;

    while (true) {
        list<Pulse> pulses;
        pulses.push_back(Pulse({ false, "broadcaster" }));
        int rxhigh = 0;
        int rxlow = 0;

        while (!pulses.empty()) {
            Pulse pulse = pulses.front();
            pulses.pop_front();

            if (pulse.ishigh) {
                ++highsum;
            }
            else {
                ++lowsum;
            }

            if (pulse.component == "zh" && pulse.ishigh) {
                cout << "ZH Input Low (" << pulse.source << ") at turn " << turn + 1 << endl;
                if (zhturns.find(pulse.source) == zhturns.end()) {
                    zhturns[pulse.source] = turn + 1;
                    if (zhturns.size() == components.at("zh").inputs.size()) {
                        goto done;
                    }
                }
            }

            Component& c = components.at(pulse.component);
            switch (c.type) {
            case 'b':
                for (string o : c.outputs) {
                    pulses.push_back(Pulse({ pulse.ishigh, o, pulse.component }));
                }
                break;

            case '%':
                if (!pulse.ishigh) {
                    c.ison = !c.ison;
                    for (string o : c.outputs) {
                        pulses.push_back(Pulse({ c.ison, o, pulse.component }));
                    }
                }
                break;

            case '&':
                if (pulse.ishigh) {
                    c.highinputs.insert(pulse.source);
                    if (c.highinputs.size() == c.inputs.size()) {
                        for (string o : c.outputs) {
                            pulses.push_back(Pulse({ false, o, pulse.component }));
                        }
                    }
                    else {
                        for (string o : c.outputs) {
                            pulses.push_back(Pulse({ true, o, pulse.component }));
                        }
                    }
                }
                else {
                    c.highinputs.erase(pulse.source);
                    for (string o : c.outputs) {
                        pulses.push_back(Pulse({ true, o, pulse.component }));
                    }
                }
                break;
            }
        }

        ++turn;
        //cout << turn << " - " << rxhigh << ":" << rxlow << endl;
        if (turn == 1000) {
            __int64 part1 = 1i64 * lowsum * highsum;
            cout << "Part 1: " << part1 << endl;
        }
    }

    done:
    // RESULTS...
    __int64 part2 = 1;
    for (auto& e : zhturns) {
        part2 = lcm(part2, e.second);
    }
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}