#include <winsqlite/winsqlite3.h>
#include "GetRowsNum.h"
#include <Windows.h>

extern sqlite3* db;

int GetRowsNum(const char tableName[]) {
	char sql[48] = "SELECT COUNT(*) FROM ";
	strcat_s(sql, tableName);
	strcat_s(sql, " ;");
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

	int row_count = 0;
	if (rc == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			row_count = sqlite3_column_int(stmt, 0);  // Кількість рядків
		}
	}
	else {
		  //std::cout << "error" << sqlite3_errmsg(db) << std::endl;
		MessageBoxA(NULL, "SQLite!", "Error", MB_OK);
	}
	sqlite3_finalize(stmt);

	return row_count;

}