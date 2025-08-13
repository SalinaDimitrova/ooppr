#include<iostream>

#include "IntCell.h"


IntCell::IntCell(int value): value(value){}

Cell* IntCell::clone() const 
{
    return new IntCell(*this);
}

std::string IntCell::toString() const 
{
    return std::to_string(value);
}

bool IntCell::equals(const Cell* other) const 
{
    if (!other || other->getType() != CellType::INT)
        return false;

    const IntCell* intOther = static_cast<const IntCell*>(other);
    return this->value == intOther->value;
}

CellType IntCell::getType() const { return CellType::INT; }