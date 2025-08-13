#pragma once

#include "../Cell.h"

class IntCell : public Cell 
{
public:
    IntCell(int value);

    Cell* clone() const override;
    std::string toString() const override;
    bool equals(const Cell* other) const override;
    
    CellType getType() const override;
    
private:
    int value;
};
