#pragma once
#include<vector>
#include<string>
#include <winsqlite/winsqlite3.h>

extern sqlite3* db;
std::vector<std::string> getTableColumns(const char tableName[]);