#pragma once
#include<vector>
#include<string>
#include <winsqlite/winsqlite3.h>
#include <iostream>
extern sqlite3* db;
std::vector<std::string> getTableColumns(const char tableName[]);