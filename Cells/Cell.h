#pragma once

#include <string>

enum class CellType 
{ 
    INT, 
    DOUBLE, 
    STRING, 
    DATE, 
    NULLTYPE 
};

class Cell 
{
public:
    virtual Cell* clone() const = 0;
    virtual std::string toString() const = 0;
    virtual bool equals(const Cell* other) const = 0;
    virtual CellType getType() const = 0;
    virtual ~Cell() {}
};
