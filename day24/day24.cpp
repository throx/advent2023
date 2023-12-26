#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_R("(-?\\d+),\\s*(-?\\d+),\\s*(-?\\d+)\\s*@\\s*(-?\\d+),\\s*(-?\\d+),\\s*(-?\\d+)");
const double EPSILON = 0.00001;
//const __int64 START = 7;
//const __int64 END = 27;
const __int64 START = 200000000000000;
const __int64 END = 400000000000000;

int main()
{
    typedef tuple<Point3, Point3> Hail;
    vector<Hail> hails;

    typedef tuple<Point2, Point2> Hail2;
    vector<Hail2> hails2;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            Point3 pos({ stoll(sm[1]), stoll(sm[2]), stoll(sm[3]) });
            Point3 vel({ stoll(sm[4]), stoll(sm[5]), stoll(sm[6]) });

            hails.push_back({ pos, vel });

            Point2 pos2({ stoll(sm[1]), stoll(sm[2]) });
            Point2 vel2({ stoll(sm[4]), stoll(sm[5]) });

            hails2.push_back({ pos2, vel2 });
        }
        else {
            throw "BOOM";
        }
    }

    int part1 = 0;

    for (int i = 0; i < hails2.size() - 1; ++i) {
        for (int j = i + 1; j < hails2.size(); ++j) {
            Hail2 h1 = hails2[i];
            Hail2 h2 = hails2[j];
            Point2 pos1 = get<0>(h1);
            Point2 pos2 = get<0>(h2);
            Point2 vel1 = get<1>(h1);
            Point2 vel2 = get<1>(h2);

            double x1 = pos1[0];
            double y1 = pos1[1];
            double x2 = pos2[0];
            double y2 = pos2[1];
            double vx1 = vel1[0];
            double vy1 = vel1[1];
            double vx2 = vel2[0];
            double vy2 = vel2[1];

            // Det = 0 means parallel paths
            double det = vx1 * vy2 - vx2 * vy1;
            if (fabs(det) < EPSILON) {

                continue;
            }
            double invdet = 1.0 / det;

            // Find times of intersections (again, thanks Mathematica)
            double t1 = -invdet * (vy2 * x1 - vy2 * x2 - vx2 * y1 + vx2 * y2);
            double t2 = invdet * (-vy1 * x1 + vy1 * x2 + vx1 * y1 - vx1 * y2);

                // Find intersection (also, thanks Mathematica)
            double x = invdet * (-vx2 * vy1 * x1 + vx1 * vy2 * x2 + vx1 * vx2 * y1 - vx1 * vx2 * y2);
            double y = -invdet * (vy1 * vy2 * x1 - vy1 * vy2 * x2 - vx1 * vy2 * y1 + vx2 * vy1 * y2);

            if (t1 >= -EPSILON && t2 >= -EPSILON && x >= START - EPSILON && x <= END + EPSILON && y >= START - EPSILON && y <= END + EPSILON) {
                ++part1;
            }
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    //cout << "Part 2: " << 0 << endl;  See Mathematica notebook for Part 2

    cout << "Press a key to continue" << endl;
    _getch();
}