#include "GetRows.h"
#include <Windows.h>
#include <winsqlite/winsqlite3.h>
#include <iostream>
#include "Definitions.h"
#include <vector>
extern short pressed_btn;
extern sqlite3* db;
extern std::vector<std::vector<HWND>> table_matrix;
extern HWND databases_window;

void GetRows(const char tableName[]) {
	char sql[48] = "SELECT * FROM ";
	strcat_s(sql, tableName);
	strcat_s(sql, " ;");
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

	RECT parent_window;
	GetClientRect(databases_window, &parent_window);

	if (rc == SQLITE_OK) {
		size_t high_index = 1;
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			int column_count = sqlite3_column_count(stmt);
			for (int i = 0; i < column_count; i++) {
				const unsigned char* text = sqlite3_column_text(stmt, i);

				HWND row_item = CreateWindowExA(0, "STATIC", reinterpret_cast<const char*>(text), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 25 + ((parent_window.right - 50) / column_count) * i, (pressed_btn + 1) * 30 + 40 + (5 * pressed_btn) + 25 * high_index, (parent_window.right - 50) / column_count, 25, databases_window, (HMENU)tables_edit_def, NULL, NULL);
				table_matrix[high_index].push_back(row_item);

			}
			high_index++;
			std::cout << std::endl;
		}
	}
	else {
		std::cout << "error" << sqlite3_errmsg(db) << std::endl;
	}
	sqlite3_finalize(stmt);
}