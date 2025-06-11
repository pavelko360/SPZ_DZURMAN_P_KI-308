#include "Get_titles.h"
#include <Windows.h>
#include "Definitions.h"
#include <vector>
extern HWND databases_window;
extern std::vector<std::vector<HWND>> table_matrix;
extern HWND result_window;

void Get_titles(sqlite3_stmt* stmt, int hight_index) {
	int columnCount = sqlite3_column_count(stmt);
	RECT parent_window;
	GetClientRect(databases_window, &parent_window);
	table_matrix.resize(1);
	for (int i = 0; i < columnCount; ++i) {
		const char* columnName = sqlite3_column_name(stmt, i);
		HWND row_item = CreateWindowExA(0, "STATIC", columnName, WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 2 + (((parent_window.right - 4) / columnCount * i)), 104 + (25 * hight_index), (parent_window.right - 4) / sqlite3_column_count(stmt), 25, result_window, (HMENU)query_result_row_def, NULL, NULL);
		table_matrix[0].push_back(row_item);
	}
}