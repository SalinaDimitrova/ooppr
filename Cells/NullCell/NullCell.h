#pragma once

#include "../Cell.h"

class NullCell : public Cell 
{
public:
    NullCell() = default;

    Cell* clone() const override;
    std::string toString() const override;
    bool equals(const Cell* other) const override;
    
    CellType getType() const override;
};

