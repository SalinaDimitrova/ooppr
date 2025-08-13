#include "StringCell.h"

StringCell::StringCell(std::string value): value(value){}

Cell* StringCell::clone() const 
{
    return new StringCell(*this);
}

std::string StringCell::toString() const 
{
    return value;
}

bool StringCell::equals(const Cell* other) const 
{
    if (!other || other->getType() != CellType::STRING)
        return false;

    const StringCell* stringOther = static_cast<const StringCell*>(other);
    return this->value == stringOther->value;
}


CellType StringCell::getType() const { return CellType::STRING; }