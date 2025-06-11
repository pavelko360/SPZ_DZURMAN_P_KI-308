#include "Query_result.h"
#include <Windows.h>
#include "Definitions.h"
#include <vector>
extern HWND result_window;
extern HWND databases_window;
extern std::vector<std::vector<HWND>> table_matrix;

void Query_result(sqlite3_stmt* stmt, int hight_index) {
	RECT parent_window;
	GetClientRect(databases_window, &parent_window);
	table_matrix.resize(table_matrix.size() + 1);
	for (int i = 0; i < sqlite3_column_count(stmt); i++) {
		const unsigned char* text = sqlite3_column_text(stmt, i);
		HWND row_item = CreateWindowExA(0, "STATIC", reinterpret_cast<const char*>(text), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 2 + (((parent_window.right - 4) / sqlite3_column_count(stmt) * i)), 104 + (25 * hight_index), (parent_window.right - 4) / sqlite3_column_count(stmt), 25, result_window, (HMENU)query_result_row_def, NULL, NULL);

		table_matrix[table_matrix.size() - 1].push_back(row_item);

	}
}