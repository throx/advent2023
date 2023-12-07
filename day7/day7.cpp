#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <conio.h>

using namespace std;

const regex INPUT_R("(.+) (\\d+)");
int CARD_VAL[256];
int CARD_VAL2[256];

int rank1(string c) {
    sort(c.begin(), c.end());

    // FIVE
    if (c[0] == c[1] &&
        c[0] == c[2] &&
        c[0] == c[3] &&
        c[0] == c[4]) return 7;

    // FOUR
    if (c[0] == c[1] &&
        c[0] == c[2] &&
        c[0] == c[3]) return 6;
    if (c[1] == c[2] &&
        c[1] == c[3] &&
        c[1] == c[4]) return 6;

    // FULL HOUSE
    if (c[0] == c[1] &&
        c[0] == c[2] &&
        c[3] == c[4]) return 5;
    if (c[0] == c[1] &&
        c[2] == c[3] &&
        c[2] == c[4]) return 5;

    // THREE
    if (c[0] == c[1] &&
        c[0] == c[2]) return 4;
    if (c[1] == c[2] &&
        c[1] == c[3]) return 4;
    if (c[2] == c[3] &&
        c[2] == c[4]) return 4;

    // TWO PAIR
    if (c[0] == c[1] &&
        c[2] == c[3]) return 3;
    if (c[0] == c[1] &&
        c[3] == c[4]) return 3;
    if (c[1] == c[2] &&
        c[3] == c[4]) return 3;

    // PAIR
    if (c[0] == c[1]) return 2;
    if (c[1] == c[2]) return 2;
    if (c[2] == c[3]) return 2;
    if (c[3] == c[4]) return 2;

    // HIGH CARD
    return 1;
}

int rank2(string c) {

    // Find the Jacks
    int j = c.find('J');

    if (j != string::npos) {

        // If we found one, loop through all possible values and recurse
        int max_rank = 0;
        for (char x : "23456789TQKA") {
            c[j] = x;
            max_rank = max(rank2(c), max_rank);
        }
        return max_rank;
    }

    // No Jacks, normal ranking
    return rank1(c);
}

struct Hand {
    string cards;
    int bid;
    int rank;

    Hand(const string& c, int b) {
        cards = c;
        bid = b;
        rank = rank1(c);
    }

    bool operator< (const Hand& other) const {
        if (rank < other.rank) return true;
        if (rank > other.rank) return false;

        for (int i = 0; i < 5; ++i) {
            if (CARD_VAL[cards[i]] < CARD_VAL[other.cards[i]]) return true;
            if (CARD_VAL[cards[i]] > CARD_VAL[other.cards[i]]) return false;
        }

        return false;
    }

    bool operator== (const Hand& other) const {
        return cards == other.cards;
    }
};

struct Hand2 {
    string cards;
    int bid;
    int rank;

    Hand2(const string& c, int b) {
        cards = c;
        bid = b;
        rank = rank2(c);
    }

    bool operator< (const Hand2& other) const {
        if (rank < other.rank) return true;
        if (rank > other.rank) return false;

        for (int i = 0; i < 5; ++i) {
            if (CARD_VAL2[cards[i]] < CARD_VAL2[other.cards[i]]) return true;
            if (CARD_VAL2[cards[i]] > CARD_VAL2[other.cards[i]]) return false;
        }

        return false;
    }

    bool operator== (const Hand2& other) const {
        return cards == other.cards;
    }
};

int main()
{
    CARD_VAL['2'] = 1;
    CARD_VAL['3'] = 2;
    CARD_VAL['4'] = 3;
    CARD_VAL['5'] = 4;
    CARD_VAL['6'] = 5;
    CARD_VAL['7'] = 6;
    CARD_VAL['8'] = 7;
    CARD_VAL['9'] = 8;
    CARD_VAL['T'] = 9;
    CARD_VAL['J'] = 10;
    CARD_VAL['Q'] = 11;
    CARD_VAL['K'] = 12;
    CARD_VAL['A'] = 13;

    CARD_VAL2['J'] = 1;
    CARD_VAL2['2'] = 2;
    CARD_VAL2['3'] = 3;
    CARD_VAL2['4'] = 4;
    CARD_VAL2['5'] = 5;
    CARD_VAL2['6'] = 6;
    CARD_VAL2['7'] = 7;
    CARD_VAL2['8'] = 8;
    CARD_VAL2['9'] = 9;
    CARD_VAL2['T'] = 10;
    CARD_VAL2['Q'] = 11;
    CARD_VAL2['K'] = 12;
    CARD_VAL2['A'] = 13;

    // Set is sorted, which does the heavy lifting for us
    set<Hand> hands;
    set<Hand2> hands2;

    // Read data
    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            Hand h({ sm[1], stoi(sm[2]) });
            hands.insert(h);

            Hand2 h2({ sm[1], stoi(sm[2]) });
            hands2.insert(h2);
        }
    }

    // Compute scores
    int part1 = 0;
    int r = 1;
    for (auto& hand : hands) {
        part1 += r * hand.bid;
        ++r;
    }

    int part2 = 0;
    r = 1;
    for (auto& hand2 : hands2) {
        part2 += r * hand2.bid;
        ++r;
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}