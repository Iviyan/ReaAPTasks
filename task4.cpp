#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <clocale>
#include <iomanip>
#include <fstream>

// Comment out the line below to use standard input instead of _getch().
#define USEGETCH

#ifdef USEGETCH
#include "conio.h"
#endif

using std::cout, std::cin, std::endl, std::string, std::vector;

enum class rps { rock, paper, scissors };
enum class gameResult { win, lose, tie };

struct participant {
    string name;
    int place = 0;
    rps move = rps::rock;
};

// This method should work like the Console.ReadKey(true) in C# if USEGETCH defined.
#ifndef USEGETCH
char readChar() {
#else
char readChar(bool newLineAfterInput = true) {
#endif
    char c;
#ifndef USEGETCH
    cin >> c;
#else
    cout.flush();
    c = _getch();
    if (newLineAfterInput) cout << endl;
#endif
    return c;
}

gameResult getResult(rps r1, rps r2) {
    if (r1 == r2) return gameResult::tie;
    if (r1 == rps::rock) {
        if (r2 == rps::paper) return gameResult::lose;
        if (r2 == rps::scissors) return gameResult::win;
    }
    else if (r1 == rps::paper) {
        if (r2 == rps::rock) return gameResult::win;
        if (r2 == rps::scissors) return gameResult::lose;
    }
    else if (r1 == rps::scissors) {
        if (r2 == rps::rock) return gameResult::lose;
        if (r2 == rps::paper) return gameResult::win;
    }
    return gameResult::tie; // unreachable code
}

std::default_random_engine rnd;

int _main() {
    setlocale(LC_ALL, "");

    auto rd = std::random_device{};
    rnd = std::default_random_engine{rd()};

    int participantsCount = 0;
    do {
        cout << "Enter the number of participants: ";
        cin >> participantsCount;
    }
    while (participantsCount < 2);

    auto *participants = new participant[participantsCount];
    for (int i = 0; i < participantsCount; ++i) {
        participant &p = participants[i];
        p.name = string("Participant ") + std::to_string(i + 1);
    }

    cout << "Choose the names of the participants? (y/n): ";
    string chooseNames;
    cin >> chooseNames;
    if (chooseNames == "y") {
        for (int i = 0; i < participantsCount; ++i) {
            cout << "Enter the name of the participant " << i + 1 << ": ";
            string name;
            cin >> name;
            participants[i].name = name;
        }
    }

    vector<participant*> roundParticipants{};
    roundParticipants.reserve(participantsCount);
    for (int i = 0; i < participantsCount; ++i)
        roundParticipants.push_back(&participants[i]);

    cout << endl;
    int round = 1;
    for (;; round++) {
        cout << "Round " << round << endl;

        vector<participant*> nextRoundParticipants{};

        size_t roundParticipantsCount = roundParticipants.size();
        std::shuffle(roundParticipants.begin(), roundParticipants.end(), rnd);

        for (int i = 0; i < roundParticipantsCount - 1; i += 2) {
            cout << "> " << roundParticipants[i]->name << " <-> " << roundParticipants[i + 1]->name << endl;
        }
        if (roundParticipantsCount % 2 == 1) {
            participant *p = roundParticipants[roundParticipantsCount - 1];
            cout << "> " << p->name << " - skips the round" << endl;
            roundParticipantsCount -= 1;
            nextRoundParticipants.push_back(p);
        }

        cout << endl << "Choose a move (r - rock, p - paper, s - scissors)" << endl;

        for (int i = 0; i < roundParticipantsCount; ++i) {
            participant *p = roundParticipants[i];
            for (;;) {
                cout << p->name << " > ";
                char mc = readChar();
                if (mc == 'r') p->move = rps::rock;
                else if (mc == 'p') p->move = rps::paper;
                else if (mc == 's') p->move = rps::scissors;
                else continue;
                break;
            }
        }

        cout << endl << "Winners / Tie" << endl;

        for (int i = 0; i < roundParticipantsCount; i += 2) {
            auto p1 = roundParticipants[i];
            auto p2 = roundParticipants[i + 1];

            auto result = getResult(p1->move, p2->move);
            if (result == gameResult::tie) {
                cout << "> " << p1->name << " <-> " << p2->name << " - tie" << endl;
                nextRoundParticipants.push_back(p1);
                nextRoundParticipants.push_back(p2);
            }
            else {
                auto pwinIndex = result == gameResult::win ? i : i + 1;
                auto plostIndex = result == gameResult::win ? i + 1 : i;
                auto pwin = roundParticipants[pwinIndex];
                auto plost = roundParticipants[plostIndex];
                cout << "> " << pwin->name << endl;
                plost->place = round;
                nextRoundParticipants.push_back(pwin);
            }
        }

        roundParticipants = nextRoundParticipants;

        if (roundParticipants.size() == 1) {
            auto p = roundParticipants[0];
            p->place = round + 1;
            cout << endl << "Winner: " << p->name << endl << endl;
            break;
        }

        cout << endl;
    }

    std::sort(participants, participants + participantsCount,
              [] (participant const& a, participant const& b) { return a.place > b.place; });

    int ratingDigits = (int)ceil(log10(round));

    cout << "Rating" << endl;
    for (int i = 0; i < participantsCount; ++i) {
        auto &p = participants[i];
        cout << std::setw(ratingDigits) << p.place << " - " << p.name << endl;
    }

    std::ofstream outputFile("game result.txt");
    if (outputFile.is_open()) {
        outputFile << "Rating" << endl;
        for (int i = 0; i < participantsCount; ++i) {
            auto &p = participants[i];
            outputFile << std::setw(ratingDigits) << p.place << " - " << p.name << endl;
        }
    }

    delete[] participants;
    return 0;
}