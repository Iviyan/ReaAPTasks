#include <iostream>
#include <vector>
#include <clocale>

using std::wcout, std::wcin, std::endl, std::wstring, std::vector;

enum class rps { rock, paper, scissors };

struct participant {
    int number;
    wstring name;
    int place;
    rps move;
};

wchar_t readChar() {
    wchar_t c;
    wcin >> c;
    return c;
    //return _getch();
}

int _main() {
    setlocale(LC_ALL, "");

    std::cout << "Введите количество участников: ";
    int participantsNumber = 0;;

    wcin >> participantsNumber;

    auto *participants = new participant[participantsNumber];
    for (int i = 0; i < participantsNumber; ++i) {
        participant &p = participants[i];
        p.number = i + 1;
        p.name = wstring(L"Участник ") + std::to_wstring(i + 1);
    }

    std::cout << "Выбрать имена участников? (да, нет): ";
    wstring chooseNames;
    wcin >> chooseNames;
    if (chooseNames == L"да") {
        for (int i = 0; i < participantsNumber; ++i) {
            wcout << "Введите имя участника " << i + 1 << ": ";
            wstring name;
            wcin >> name;
        }
    }

    wcout << "Выберите ход (к - камень, н - ножницы, б - бумага)" << endl;

    for (int i = 0; i < participantsNumber; ++i) {
        participant &p = participants[i];
        wcout << p.name << " > ";
        wchar_t mc = readChar();
        rps move;
        if (mc == L'к') move = rps::rock;
        else if (mc == L'н') move = rps::scissors;
        else if (mc == L'б') move = rps::paper;
    }


    delete[] participants;
    return 0;
}
