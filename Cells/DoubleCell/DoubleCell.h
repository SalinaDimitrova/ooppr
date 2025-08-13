#pragma once

#include "../Cell.h"

class DoubleCell : public Cell 
{
public:
    DoubleCell(double v);
    
    Cell* clone() const override;
    std::string toString() const override;
    bool equals(const Cell* other) const override;
    
    CellType getType() const override;

private:
    double value;
};
