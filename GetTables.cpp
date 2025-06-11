#include "GetTables.h"
#include "Definitions.h"
#include "MoveBlock.h"
#include <iostream>
void GetTables(std::string dbname, int index) {
	string bdName;
	string sql_query;
	sqlite3_close(db);
	string dbName = "databases/";
	dbName += dbname.c_str();

	if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
		std::cout << "Open error" << sqlite3_errmsg(db) << std::endl;
	}

	else {
		sqlite3_stmt* stmt;
		bool hasRows = false;
		const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";
		if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
			std::cout << "prepare errror " << std::endl;
			sqlite3_close(db);
		}
		else {
			int tbls = 0, dbs = index;
			int table_button_position_y, table_button_position_x;

			while (sqlite3_step(stmt) == SQLITE_ROW) {

				table_button_position_y = (30 * (index + 1)) + 10 + 5 * index;
				table_button_position_x = 25 + (110 * tbls) + tbls * 1;
				tbls++;
				dbs++;


				const char* tblName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
				HWND Table = CreateWindowExA(0, "BUTTON", tblName, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, table_button_position_x, table_button_position_y, 110, 30, databases_window, (HMENU)databases_edit_def, NULL, NULL);
				db_tables.push_back(Table);

			}
			MoveBlock(index);

		}
		sqlite3_finalize(stmt);
	}
}