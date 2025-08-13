#include<iostream>

#include "DoubleCell.h"


DoubleCell::DoubleCell(double value): value(value){}

Cell* DoubleCell::clone() const 
{
    return new DoubleCell(*this);
}

std::string DoubleCell::toString() const 
{
    return std::to_string(value);
}

bool DoubleCell::equals(const Cell* other) const 
{
    if (!other || other->getType() != CellType::DOUBLE)
        return false;

    const DoubleCell* doubleOther = static_cast<const DoubleCell*>(other);
    return this->value == doubleOther->value;
}

CellType DoubleCell::getType() const { return CellType::DOUBLE; }