#include "Row.h"

#include <string>

Row::Row(const std::vector<Cell*>& data) 
{
    for (size_t i = 0; i < data.size(); ++i) 
    {
        Cell* cell = data[i];
        cells.push_back(cell ? cell->clone() : nullptr);
    }
}

Row::Row(const Row& other) 
{
    copyFrom(other);
}

Row& Row::operator=(const Row& other) 
{
    if (this != &other) 
    {
        Row temp(other);   
        std::swap(cells, temp.cells); 
    }
    return *this;
}

Row::~Row() 
{
    freeDynamic();
}

void Row::copyFrom(const Row& other) 
{
    std::vector<Cell*> temp;
    temp.reserve(other.cells.size());

    for (std::size_t i = 0; i < other.cells.size(); ++i) 
    {
        temp.push_back(other.cells[i] ? other.cells[i]->clone() : nullptr);
    }

    std::swap(cells, temp);
}

void Row::freeDynamic() 
{
    for (size_t i = 0; i < cells.size(); ++i) 
    {
        delete cells[i];
    }

    cells.clear();
}

std::string Row::toString() const 
{
    std::string result;
    for (size_t i = 0; i < cells.size(); ++i) 
    {
        if (i > 0) 
            result += ", ";
        result += cells[i] ? cells[i]->toString() : "NULL";
    }
    return result;
}
