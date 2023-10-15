#include <iostream>

int sumDigits(int number);
bool isLeapYear(int year);
int getNumberOfDays(int month, int year);

int main1() {

    std::cout << "Введите год: ";
    int year = 0;;

    if (!(std::cin >> year)) {
        std::cout << "Некорректный ввод" << std::endl;
        return -1;
    }
    std::cout << year << std::endl;

    int result = 0;
    for (int month = 1; month <= 12; month++) {
        int daysCount = getNumberOfDays(month, year);

        for (int day = 1; day <= daysCount; day++) {
            result += sumDigits(day);
        }
    }

    std::cout << "Сумма цифр всех дней каждого месяца в году: " << result << std::endl;
    return 0;
}

int sumDigits(int number) {
    int result = 0;
    while (number > 0) {
        result += number % 10;
        number /= 10;
    }
    return result;
}


bool isLeapYear(int year) {
    return ((year & 3) == 0 && ((year % 25) != 0 || (year & 15) == 0));
}

int getNumberOfDays(int month, int year) {
    if (month == 2)
        return isLeapYear(year) ? 29 : 28;

    return month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12
           ? 31 : 30;
}