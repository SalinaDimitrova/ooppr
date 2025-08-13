#include "DateCell.h"

DateCell::DateCell(int day, int month, int year) : value(day, month, year) {}
DateCell::DateCell(const Date& date) : value(date) {}

Cell* DateCell::clone() const 
{
    return new DateCell(*this);
}

std::string DateCell::toString() const 
{
    return value.toString();
}

bool DateCell::equals(const Cell* other) const 
{
    if (!other || other->getType() != CellType::DATE)
        return false;

    const DateCell* dateOther = static_cast<const DateCell*>(other);
    return this->value == dateOther->value;
}

CellType DateCell::getType() const { return CellType::DATE; }
