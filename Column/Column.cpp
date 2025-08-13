#include "Column.h"

#include <stdexcept>

Column::Column() : name(""), type("") {}

Column::Column(const std::string& name, const std::string& type) 
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

void Column::setType(const std::string& type) 
{
    if (type != "int" && type != "double" && type != "string" && type != "date") 
        throw std::invalid_argument("Unsupported column type!");
    
    this->type = type;
}

bool Column::operator==(const Column& rhs) const 
{
    return this->name == rhs.name && this->type == rhs.type;
}

bool Column::operator!=(const Column& rhs) const 
{
    return !(*this == rhs);
}


