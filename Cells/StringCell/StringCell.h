#pragma once

#include "../Cell.h"

class StringCell : public Cell 
{
public:
    StringCell(std::string value);

    Cell* clone() const override;
    std::string toString() const override;
    bool equals(const Cell*) const override;
    
    CellType getType() const override;

private:
    std::string value;
};
