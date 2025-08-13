#pragma once

#include <string>

class Date 
{
public:
    Date(int day = 1, int month = 1, int year = 1900);

    bool operator==(const Date& rhs) const;
    bool operator!=(const Date& rhs) const;
    std::string toString() const;

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    
private:
    int day;
    int month;
    int year;

    bool isValid() const;
};

