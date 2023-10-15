#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using std::swap, std::cout, std::cin, std::endl, std::pair, std::vector, std::string, std::to_string;

int nod(int a, int b) {
    for (;;) {
        a %= b;
        if (!a) return abs(b);
        b %= a;
        if (!b) return abs(a);
    }
}

int nok(int a, int b) {
    return abs(a * b) / nod(a, b);
}

vector<int> factorize(int x) {
    vector<int> factors;

    for (int i = 2; i <= sqrt(x); i++) {
        while (x % i == 0) {
            factors.push_back(i);
            x /= i;
        }
    }

    if (x != 1) {
        factors.push_back(x);
    }

    return factors;
}

string fractionToString(pair<int, int> f) {
    if (f.first == 0) return "0";
    if (f.second == 1) return to_string(f.first);
    return to_string(f.first) + "/" + to_string(f.second);
}

pair<int, int> sumFractions(pair<int, int> f1, pair<int, int> f2) {
    int denominator = nok(f1.second, f2.second);
    int numerator = f1.first * (denominator / f1.second) + f2.first * (denominator / f2.second);
    return {numerator, denominator};
}

pair<int, int> subFractions(pair<int, int> f1, pair<int, int> f2) {
    int denominator = nok(f1.second, f2.second);
    int numerator = f1.first * (denominator / f1.second) - f2.first * (denominator / f2.second);
    return {numerator, denominator};
}

pair<int, int> mulFractions(pair<int, int> f1, pair<int, int> f2) {
    return {f1.first * f2.first, f1.second * f2.second};
}

pair<int, int> divFractions(pair<int, int> f1, pair<int, int> f2) {
    return mulFractions(f1, {f2.second, f2.first});
}

pair<int, int> reduceFraction(pair<int, int> f) {
    int divider = nod(f.first, f.second);
    return {f.first / divider, f.second / divider};
}

pair<int, int>& normalizeFraction(pair<int, int> &f) {
    if (f.first < 0 && f.second < 0 || f.first > 0 && f.second < 0) {
        f.first = -f.first;
        f.second = -f.second;
    }
    return f;
}

int _main() {
    /*auto fv = factorize(201);
    for (int n : fv) cout << n << " ";
    return 0;*/

    pair<int, int> fraction1, fraction2, fractionResult;

    cout << "Введите последовательно числитель и знаменатель первой дроби: ";
    cin >> fraction1.first >> fraction1.second;
    normalizeFraction(fraction1);

    if (fraction1.second == 0) {
        cout << "Знаменателем дроби не может быть 0." << endl;
        return -1;
    }

    cout << "Введите последовательно числитель и знаменатель второй дроби: ";
    cin >> fraction2.first >> fraction2.second;
    normalizeFraction(fraction2);

    if (fraction2.second == 0) {
        cout << "Знаменателем дроби не может быть 0." << endl;
        return -1;
    }

    cout << "Введите операцию (+ - * /): ";
    char op = 0;
    cin >> op;

    if (op == '+') fractionResult = sumFractions(fraction1, fraction2);
    else if (op == '-') fractionResult = subFractions(fraction1, fraction2);
    else if (op == '*') fractionResult = mulFractions(fraction1, fraction2);
    else if (op == '/') fractionResult = divFractions(fraction1, fraction2);
    else {
        cout << "Некорретный ввод" << endl;
        return -1;
    }

    cout << "Результат: " << fractionToString(fractionResult)
         << " = " << fractionToString(reduceFraction(fractionResult));

    return 0;
}