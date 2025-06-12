#include "DbManagementProcedure.h"
#include "Definitions.h"
#include "ClearRow.h"
#include "ClearTables.h"
#include "MoveBlock.h"
#include "GetTables.h"
#include "GetRows.h"
#include "GetRowsNum.h"
#include "getTableColumns.h"

extern std::vector<HWND>databases;
extern std::vector<HWND>db_tables;;
extern short active_table, pressed_btn;
extern HFONT default_font, bold_font;

LRESULT CALLBACK DbManagementProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT rc;
		GetClientRect(hWnd, &rc);

		// Фон
		HBRUSH bgBrush = CreateSolidBrush(RGB(220, 220, 220)); //фон 
		FillRect(hdc, &rc, bgBrush);
		DeleteObject(bgBrush);

		// Рамка
		HPEN pen = CreatePen(PS_SOLID, 3, RGB(180, 180, 180));
		HGDIOBJ oldPen = SelectObject(hdc, pen);
		HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
		RoundRect(hdc, rc.left, rc.top, rc.right - 1, rc.bottom - 1, 15, 15);
		SelectObject(hdc, oldPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(pen);
	}
	else if (msg == WM_DRAWITEM) {
		LPDRAWITEMSTRUCT lpDraw = (LPDRAWITEMSTRUCT)lp;


		HDC hdc = lpDraw->hDC;
		RECT rc = lpDraw->rcItem;
		UINT id = lpDraw->CtlID;
		HBRUSH bgBrush;
		HPEN hPen;
		if (lpDraw->CtlID == databases_edit_def) {
			if (active_table >= 0 && lpDraw->hwndItem == db_tables[active_table]) {
				bgBrush = CreateSolidBrush(RGB(253, 253, 235));
				FillRect(hdc, &rc, bgBrush);
				DeleteObject(bgBrush);
				hPen = CreatePen(PS_SOLID, 4, RGB(253, 253, 235));
				SetTextColor(hdc, RGB(40, 130, 220));
			}
			else {
				bgBrush = CreateSolidBrush(RGB(127, 178, 228));
				FillRect(hdc, &rc, bgBrush);
				DeleteObject(bgBrush);
				hPen = CreatePen(PS_SOLID, 4, RGB(30, 120, 255));
				SetTextColor(hdc, RGB(253, 253, 235));
			}



			SelectObject(hdc, default_font);


			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

			// Зліва
			MoveToEx(hdc, rc.left, rc.top, NULL);
			LineTo(hdc, rc.left, rc.bottom);

			// Знизу
			MoveToEx(hdc, rc.left, rc.bottom, NULL);
			LineTo(hdc, rc.right, rc.bottom);

			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);
		}
		else if (lpDraw->CtlID == tables_edit_def) {
			SelectObject(hdc, default_font);
			HBRUSH hBrushBackground = CreateSolidBrush(RGB(255, 255, 255));
			FillRect(hdc, &rc, hBrushBackground);
			DeleteObject(hBrushBackground);

			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(220, 220, 220));
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);

			SetTextColor(hdc, RGB(0, 0, 0));
		}
		else if (lpDraw->CtlID == tables_titles_def) {
			SelectObject(hdc, bold_font);
			HBRUSH hBrushBackground = CreateSolidBrush(RGB(253, 253, 235));
			FillRect(hdc, &rc, hBrushBackground);
			DeleteObject(hBrushBackground);
			SetTextColor(hdc, RGB(0, 0, 0));
		}
		else {
			BOOL pressed = (lpDraw->itemState & ODS_SELECTED);
			HBRUSH bgBrush = CreateSolidBrush(pressed ? RGB(150, 150, 150) : RGB(235, 235, 235));
			FillRect(hdc, &rc, bgBrush);
			DeleteObject(bgBrush);
			SelectObject(hdc, default_font);

			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(180, 180, 180));
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);

			SetTextColor(hdc, RGB(0, 0, 0));
		}


		SetBkMode(hdc, TRANSPARENT);

		wchar_t text[32]{ 0 };
		SIZE textSize = { 0 };
		GetTextExtentPoint32(hdc, text, lstrlen(text), &textSize);
		GetWindowText(lpDraw->hwndItem, text, 32);

		TextOut(hdc, 6, (30 - textSize.cy) / 4, text, lstrlen(text));
	}
	else if (msg == WM_COMMAND) {
		int controlID = LOWORD(wp);
		HWND hwndClicked = (HWND)lp;  // Кнопка, яку натиснули
		if (controlID == btn_db_manage_def) {
			ClearRow();
			for (size_t i = 0; i < databases.size(); ++i) {
				if (databases[i] == hwndClicked) {
					if (pressed_btn == i) {
						pressed_btn = -1;
						ClearTables();
						MoveBlock(pressed_btn);
						active_table = -1;
						ClearRow();
					}
					else {
						pressed_btn = i;
						ClearTables();
						MoveBlock(pressed_btn);
						GetTables(databases_name[i], i);
					}
					if (active_table != -1) {
						ClearRow();
						MoveBlock(pressed_btn);
					}
				}
			}
		}
		else if (controlID == databases_edit_def) {
			RECT parent_window;
			GetClientRect(databases_window, &parent_window);
			for (size_t i = 0; i < db_tables.size(); i++)
			{
				if (db_tables[i] == hwndClicked && active_table != i) {
					ClearRow();
					active_table >= 0 ? InvalidateRect(db_tables[active_table], NULL, TRUE) : NULL;

					active_table = i;
					InvalidateRect(db_tables[active_table], NULL, TRUE);
					char table_name[32]{ 0 };
					GetWindowTextA(db_tables[i], table_name, 32);
					std::vector<std::string> cols = getTableColumns(table_name);
					int rows_num = GetRowsNum(table_name) + 1;
					table_matrix.resize(rows_num);

					MoveBlock(pressed_btn);

					for (size_t j = 0; j < cols.size(); j++) {
						std::cout << parent_window.right << std::endl;
						HWND row_item = CreateWindowExA(0, "STATIC", cols[j].c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 25 + ((parent_window.right - 50) / cols.size()) * j, (pressed_btn + 1) * 30 + 40 + 5 * pressed_btn, (parent_window.right - 50) / cols.size(), 25, databases_window, (HMENU)tables_titles_def, NULL, NULL);
						table_matrix[0].push_back(row_item);
					}

					GetRows(table_name);
				}
				else if (active_table == i) {
					active_table = -1;
					InvalidateRect(db_tables[i], NULL, TRUE);
					ClearRow();
				}
			}
		}
	}
	else if (msg == WM_SIZE) {
		RECT wind;
		GetClientRect(hWnd, &wind);
		HRGN hRgn = CreateRoundRectRgn(0, 0, wind.right, wind.bottom, 9, 9);
		SetWindowRgn(hWnd, hRgn, TRUE);
		DeleteObject(hRgn);
		InvalidateRect(databases_window, NULL, TRUE);
	}
	else return DefWindowProc(hWnd, msg, wp, lp);
}

