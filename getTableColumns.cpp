#include "getTableColumns.h"
#include <Windows.h>

std::vector<std::string> getTableColumns(const char tableName[]) {

	std::vector<std::string> columns;
	char sql[48] = "PRAGMA table_info(";
	strcat_s(sql, tableName);
	strcat_s(sql, ");");
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		//std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
		MessageBoxA(NULL, "Open db error!", "Error", MB_OK);
		return columns;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		const unsigned char* colName = sqlite3_column_text(stmt, 1);
		if (colName)
			columns.push_back(reinterpret_cast<const char*>(colName));
	}

	sqlite3_finalize(stmt);
	return columns;
}