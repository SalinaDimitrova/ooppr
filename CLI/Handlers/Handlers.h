#pragma once
#include "../Context.h"
#include <string>
#include <vector>

class Table;

Table* requireTable(Context& ctx, const std::string& name);

void handleInsert (Context& ctx, const std::vector<std::string>& args);
void handleModify (Context& ctx, const std::vector<std::string>& args);
void handleSelect (Context& ctx, const std::vector<std::string>& args);
void handleUpdate (Context& ctx, const std::vector<std::string>& args);
void handleDelete (Context& ctx, const std::vector<std::string>& args);
void handleAddColumn(Context& ctx, const std::vector<std::string>& args);
void handleExport(Context& ctx, const std::vector<std::string>& args);
