#include "NullCell.h"

Cell* NullCell::clone() const 
{
    return new NullCell(*this);
}

std::string NullCell::toString() const 
{
    return "NULL";
}

bool NullCell::equals(const Cell* other) const 
{
    return other && other->getType() == CellType::NULLTYPE;
}

CellType NullCell::getType() const { return CellType::NULLTYPE; }
