#pragma once

#include "../Cell.h"
#include "Date.h"

class DateCell : public Cell 
{
public:
    DateCell(int day, int month, int year);
    DateCell(const Date& date);

    Cell* clone() const override;
    std::string toString() const override;
    bool equals(const Cell* other) const override;
    
    CellType getType() const override;

private:
    Date value;
};
