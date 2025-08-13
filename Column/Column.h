#pragma once

#include <string>

class Column 
{
public:
    Column();
    Column(const std::string& name, const std::string& type);

    void setName(const std::string& name);
    void setType(const std::string& type);

    const std::string& getName() const { return name; }
    const std::string& getType() const { return type; }

    bool operator==(const Column& rhs) const;
    bool operator!=(const Column& rhs) const;

private: 
    std::string name;
    std::string type;
};

