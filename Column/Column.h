#pragma once

#include <string>
#include "../Cells/Cell.h"

class Column 
{
public:
    Column();
    Column(const std::string& name, CellType type);

    void setName(const std::string& name);
    void setType(CellType type);

    const std::string& getName() const { return name; }
    CellType getType() const { return type; }

    static CellType typeFromString(const std::string& s); // "int" -> CellType::INT
    static const char* typeToString(CellType t);   

    bool operator==(const Column& rhs) const;
    bool operator!=(const Column& rhs) const;

private: 
    std::string name;
    CellType type{CellType::STRING};
};

