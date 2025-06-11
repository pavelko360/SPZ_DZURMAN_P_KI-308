#include "ClearLeft.h"
#include <winsqlite/winsqlite3.h>
#include <Windows.h>
#include <vector>

extern std::vector<HWND>db_tables_left;
extern std::vector<HWND>db_window;
extern HWND left_panel;

void ClearLeft(size_t i) {
	RECT left_rec;
	GetClientRect(left_panel, &left_rec);
	for (size_t k = 0; k < db_tables_left.size(); k++)
	{
		DestroyWindow(db_tables_left[k]);
	}
	db_tables_left.clear();

	for (size_t j = i, k = i / 2; j < db_window.size(); j += 2, k++)
	{
		MoveWindow(db_window[j], 30, 225 + (30 * k) + (k * 2), 30, 30, true);
		MoveWindow(db_window[j - 1], 60, 225 + (30 * k) + (k * 2), left_rec.right - 100, 30, true);

	}
}