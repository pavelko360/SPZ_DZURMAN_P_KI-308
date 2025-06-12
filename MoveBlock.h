#pragma once
#include <Windows.h>

#include <vector>
#include <string>
extern std::vector<HWND>db_tables;
extern std::vector<std::vector<HWND>> table_matrix;
extern std::vector<HWND>databases;
extern std::vector<std::string> databases_name;


void MoveBlock(int index);