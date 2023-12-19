#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <list>
#include <map>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("([a-z]+)\\{(.*)\\}");
const regex STEP_R("(.)(.)(\\d+)\\:(.*)");
const regex VAL_R("\\{x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)\\}");

struct Step {
    int var;
    char comp;
    int num;
    string dest;
};

typedef list<Step> Workflow;
typedef map<string, Workflow> WorkMap;
typedef vector<int> Item;

string get_next(const Workflow& workflow, const Item& item)
{
    for (auto& step : workflow) {
        if (step.var != -1) {
            int v = item[step.var];
            switch (step.comp) {
            case '<':
                if (v < step.num) {
                    return step.dest;
                }
                break;

            case '>':
                if (v > step.num) {
                    return step.dest;
                }
                break;

            default:
                throw "BOOM";
            }
        }
        else {
            return step.dest;
        }
    }
    throw "BANG";
}

__int64 combos(const WorkMap& workmap, string flow, Item min_item, Item max_item)
{
    if (flow == "R") {
        return 0;
    }
    if (flow == "A") {
        return 1i64 * (max_item[0] - min_item[0] - 1) * (max_item[1] - min_item[1] - 1) * (max_item[2] - min_item[2] - 1) * (max_item[3] - min_item[3] - 1);
    }

    __int64 ret = 0;
    Workflow workflow = workmap.at(flow);
    for (auto& step : workflow) {
        if (step.var != -1) {
            switch (step.comp) {
            case '<':
            {
                auto new_max = max_item;
                new_max[step.var] = min(step.num, new_max[step.var]);
                ret += combos(workmap, step.dest, min_item, new_max);
                min_item[step.var] = max(step.num - 1, min_item[step.var]);
                break;
            }

            case '>':
            {
                auto new_min = min_item;
                new_min[step.var] = max(step.num, new_min[step.var]);
                ret += combos(workmap, step.dest, new_min, max_item);
                max_item[step.var] = min(step.num + 1, max_item[step.var]);
                break;
            }

            default:
                throw "BOOM";
            }
        }
        else {
            ret += combos(workmap, step.dest, min_item, max_item);
        }
    }

    return ret;
}

int main()
{
    WorkMap workmap;

    string s;
    while (getline(cin, s)) {
        if (s.empty()) {
            break;
        }

        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            Workflow workflow;
            for (string s2 : Split(sm[2], ',')) {
                smatch sm2;
                if (regex_match(s2.cbegin(), s2.cend(), sm2, STEP_R)) {
                    int var = "xmas"s.find(sm2[1].str()[0]);
                    Step step({ var, sm2[2].str()[0], stoi(sm2[3]), sm2[4]});
                    workflow.push_back(step);
                }
                else {
                    Step step({ -1, 0, -1, s2 });
                    workflow.push_back(step);
                }
            }
            workmap[sm[1]] = workflow;
        }
    }

    __int64 part1 = 0;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, VAL_R)) {
            Item item({ stoi(sm[1]), stoi(sm[2]), stoi(sm[3]), stoi(sm[4]) });
            string flow = "in";
            while (flow != "A" && flow != "R") {
                flow = get_next(workmap[flow], item);
            }

            if (flow == "A") {
                part1 += item[0];
                part1 += item[1];
                part1 += item[2];
                part1 += item[3];
            }
        }
    }

    cout << "Part 1: " << part1 << endl;

    __int64 part2 = combos(workmap, "in", Item({ 0, 0, 0, 0 }), Item({ 4001, 4001, 4001, 4001 }));

    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}