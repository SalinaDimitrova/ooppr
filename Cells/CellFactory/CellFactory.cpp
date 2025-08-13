#include "CellFactory.h"

#include <stdexcept>
#include <string>

std::string parseEscapedString(const std::string& quoted) 
{
    std::string result;
    
    for (size_t i = 1; i + 1 < quoted.length(); ++i) 
    {
        if (quoted[i] == '\\' && i + 1 < quoted.length() - 1) 
        {
            if (quoted[i + 1] == '"' || quoted[i + 1] == '\\') 
            {
                result += quoted[i + 1];
                ++i;
            } else
                result += quoted[i];
        } else
            result += quoted[i];
    }
    return result;
}

bool parseDateString(const std::string& value, int& y, int& m, int& d) 
{
    if (value.size() != 10 || value[4] != '-' || value[7] != '-') 
        return false;

    try 
    {
        y = std::stoi(value.substr(0, 4));
        m = std::stoi(value.substr(5, 2));
        d = std::stoi(value.substr(8, 2));

        if (m < 1 || m > 12 || d < 1 || d > 31)
            return false;

        return true;
    } 
    catch (...) 
    {
        return false;
    }
}

Cell* createCellFromString(const std::string& value, const std::string& type, bool strictStringParsing) 
{
    if (value == "NULL")
        return new NullCell();

    try 
    {
        if (type == "int") 
        {
            int intValue = std::stoi(value);
            return new IntCell(intValue);
        }
        if (type == "double") 
        {
            double dblValue = std::stod(value);
            return new DoubleCell(dblValue);
        }
        if (type == "string") 
        {
            if (!strictStringParsing)  
                return new StringCell(value);

            if (value.length() >= 2 && value.front() == '"' && value.back() == '"')
                return new StringCell(parseEscapedString(value));
            else
                throw std::invalid_argument("String must be quoted.");
        }
        if (type == "date") 
        {
            int y, m, d;
            if (parseDateString(value, y, m, d)) 
                return new DateCell(d, m, y);
            else 
                throw std::invalid_argument("Invalid date format.");
        }

        throw std::invalid_argument("Unknown type: " + type);

    } catch (const std::exception& e) 
    {
        throw std::invalid_argument("Failed to parse value '" + value + "' for type '" + type + "'");
    }
}
