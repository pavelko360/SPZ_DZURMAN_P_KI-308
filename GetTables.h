#pragma once
#include <string>
#include <winsqlite/winsqlite3.h>
#include <Windows.h>

#include <vector>


extern std::vector<HWND>db_tables;
extern sqlite3* db;
extern HWND databases_window;
using std::string;
void GetTables(std::string dbname, int index);