#include "ClearTables.h"
#include <Windows.h>
#include <vector>
#include <string>
extern std::vector<HWND>db_tables;
extern std::vector<std::string> databases_name;
extern std::vector<HWND>databases;

void ClearTables() {
	for (size_t j = 0; j < db_tables.size(); j++) {
		DestroyWindow(db_tables[j]);
	}
	db_tables.clear();
	for (size_t k = 0; k < databases_name.size(); k++)
	{
		MoveWindow(databases[k], 10, 35 * k + 10, 125, 30, true);
	}
}