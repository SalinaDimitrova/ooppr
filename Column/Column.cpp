#include "Column.h"

#include <stdexcept>

static std::string toLowerCopy(const std::string& s)
{
    std::string r;
    r.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) 
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c >= 'A' && c <= 'Z')
            r.push_back(static_cast<char>(c + ('a' - 'A')));
        else
            r.push_back(static_cast<char>(c));
    }
    return r;
}

Column::Column() : name(""), type(CellType::STRING) {}

Column::Column(const std::string& name, CellType type) 
{
    try 
    {
        setName(name); 
        setType(type);
    } catch (...) 
    {
        throw; 
    }
}

void Column::setName(const std::string& name) 
{
    if (name.empty()) 
        throw std::invalid_argument("Column name cannot be empty.");
    
    this->name = name;
}

void Column::setType(CellType type) 
{
    if (type == CellType::NULLTYPE)
        throw std::invalid_argument("Column type cannot be NULLTYPE.");

    this->type = type;
}

CellType Column::typeFromString(const std::string& s) 
{
    std::string t = toLowerCopy(s);
    if (t == "int")    return CellType::INT;
    if (t == "double") return CellType::DOUBLE;
    if (t == "string") return CellType::STRING;
    if (t == "date")   return CellType::DATE;
    throw std::invalid_argument("Unsupported column type: " + s);
}

const char* Column::typeToString(CellType t) 
{
    switch (t) {
        case CellType::INT:    return "int";
        case CellType::DOUBLE: return "double";
        case CellType::STRING: return "string";
        case CellType::DATE:   return "date";
        case CellType::NULLTYPE: return "null";
    }
    return "unknown";
}

bool Column::operator==(const Column& rhs) const 
{
    return this->name == rhs.name && this->type == rhs.type;
}

bool Column::operator!=(const Column& rhs) const 
{
    return !(*this == rhs);
}


