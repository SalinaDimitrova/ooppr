#pragma once
#include "../Database/Database.h"
#include <iostream>
#include <string>

struct Context 
{
    Database& db;
    std::string currentFile;
    std::istream& in;
    std::ostream& out;
    std::ostream& err;
};