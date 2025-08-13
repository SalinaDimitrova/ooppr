#include "Date.h"

#include <string>

Date::Date(int d, int m, int y) : day(d), month(m), year(y) 
{
    if (!isValid()) 
    {
        day = 1;
        month = 1;
        year = 1900;
    }
}

bool Date::isValid() const 
{
    if (year < 1900 || month < 1 || month > 12 || day < 1)
        return false;

    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
        return day <= 29;

    return day <= daysInMonth[month - 1];
}

bool Date::operator==(const Date& rhs) const 
{
    return day == rhs.day && month == rhs.month && year == rhs.year;
}

bool Date::operator!=(const Date& rhs) const 
{
    return !(*this == rhs);
}

std::string padNumber(int number, int width) 
{
    std::string str = std::to_string(number);
    while (str.length() < static_cast<size_t>(width)) 
    {
        str = "0" + str;
    }
    return str;
}

std::string Date::toString() const 
{
    return padNumber(year, 4) + "-" + padNumber(month, 2) + "-" + padNumber(day, 2);
}



