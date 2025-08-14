#pragma once
#include <vector>
#include <string>

#include "../Cells/Cell.h"
#include "../Cells/NullCell/NullCell.h" 

class Row 
{
public:
    Row() = default;
    Row(const std::vector<Cell*>& data);

    Row(const Row& other);
    Row& operator=(const Row& other);
    ~Row();

    Cell* cellAt(std::size_t index) { return cells.at(index); }
    const Cell* cellAt(std::size_t index) const { return cells.at(index); }

    void replaceCell(std::size_t index, Cell* newCell) 
    {
        delete cells.at(index);
        cells[index] = newCell;
    }

    std::size_t size() const { return cells.size(); }

    std::string toString() const;

    void appendCell(Cell* newCell) { cells.push_back(newCell); }
    void appendNull() { cells.push_back(new NullCell()); }

private:
    std::vector<Cell*> cells;

    void copyFrom(const Row& other);
    void freeDynamic();
};
