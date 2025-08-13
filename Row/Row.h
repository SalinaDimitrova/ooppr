#pragma once

#include <vector>
#include <string>

#include "../Cells/Cell.h"

class Row 
{
public:
    Row() = default;
    Row(const std::vector<Cell*>& data);

    Row(const Row& other);
    Row& operator=(const Row& other);
    ~Row();

    const std::vector<Cell*>& getCells() const { return cells; }
    std::vector<Cell*>& getCells() { return cells; }

    std::string toString() const;

private:
    std::vector<Cell*> cells;

    void copyFrom(const Row& other);
    void freeDynamic();
};

