#include "LeftPanelProcedure.h"
#include "Definitions.h"
#include <iostream>
#include "ClearLeft.h"
#include <string>
#include <vector>>
#include <winsqlite/winsqlite3.h>
extern HWND hNoDB;
extern HICON g_hLogo;
extern HICON hIconExtend;
extern HFONT default_font;
extern UINT active_table_left;
extern std::vector<HWND>db_window;
extern std::vector<HWND>db_tables_left;
extern short prev_table;
extern HWND left_panel;
extern sqlite3* db;

LRESULT CALLBACK LeftPanelProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	if (msg == WM_CTLCOLORSTATIC) {
		if ((HWND)lp == hNoDB) {
			HDC hdcStatic = (HDC)wp;
			SetTextColor(hdcStatic, RGB(50, 100, 135)); // колір тексту текст
			SetBkMode(hdcStatic, TRANSPARENT);
			return (INT_PTR)CreateSolidBrush(RGB(255, 255, 255)); // колір фону
		}
		if (GetDlgCtrlID(HWND(lp)) == show_db_def) {
			SetTextColor(HDC(wp), RGB(0, 0, 0));
			SetBkColor(HDC(wp), RGB(220, 220, 220));
			static HBRUSH hBrush = CreateSolidBrush(RGB(220, 220, 220));
			return (INT_PTR)hBrush;
		}
	}
	else if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);


		RECT rect;
		GetClientRect(hWnd, &rect);

		HBRUSH bgBrush = CreateSolidBrush(RGB(220, 220, 220)); // колір фону
		FillRect(hdc, &rect, bgBrush);

		HBRUSH hBrush = CreateSolidBrush(RGB(111, 111, 111)); //колір полоски
		RECT right_border = { rect.right - 2, 0, rect.right, rect.bottom };
		FillRect(hdc, &right_border, hBrush);
		DeleteObject(hBrush);

		if (g_hLogo) {
			DrawIconEx(hdc, (rect.right - 196) / 2 - 10, 20, g_hLogo, 196, 196, 0, NULL, DI_NORMAL);
		}
		else std::cout << "error" << std::endl;

		EndPaint(hWnd, &ps);
	}
	else if (msg == WM_CREATE) {
		hIconExtend = (HICON)LoadImage(GetModuleHandle(NULL), L"..\\..\\png\\btn_extend.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		if (!hIconExtend) {
			MessageBox(NULL, L"Не вдалося завантажити іконку! hIconExtend", L"Помилка", MB_OK | MB_ICONERROR);
		}
		g_hLogo = (HICON)LoadImage(NULL, L"..\\..\\png\\left_logo2.ico", IMAGE_ICON, 196, 196, LR_LOADFROMFILE);
		if (!g_hLogo) {
			MessageBox(NULL, L"Не вдалося завантажити іконку! g_hLogo", L"Помилка", MB_OK | MB_ICONERROR);
		}
	}
	else if (msg == WM_DRAWITEM) {
		LPDRAWITEMSTRUCT lpDraw = (LPDRAWITEMSTRUCT)lp;
		HDC hdc = lpDraw->hDC;
		RECT rc = lpDraw->rcItem;
		UINT id = lpDraw->CtlID;
		HBRUSH bgBrush;
		HPEN hPen;
		if (lpDraw->CtlID == btn_show_table_def) {
			// Фон
			bgBrush = CreateSolidBrush(RGB(230, 230, 230));
			FillRect(hdc, &rc, bgBrush);
			DeleteObject(bgBrush);

			// Текст і іконка
			SelectObject(hdc, default_font);
			SetTextColor(hdc, RGB(0, 0, 0));
			SetBkMode(hdc, TRANSPARENT);

			LPCWSTR text = L"";
			HICON hIcon = NULL;

			SIZE textSize = { 0 };
			GetTextExtentPoint32(hdc, text, lstrlen(text), &textSize);

			int y = (30 - textSize.cy) / 2;
			int iconX = (lpDraw->rcItem.right - lpDraw->rcItem.left - 16) / 2;
			int iconY = (lpDraw->rcItem.bottom - lpDraw->rcItem.top - 16) / 2;
			DrawIconEx(lpDraw->hDC, lpDraw->rcItem.left + iconX, lpDraw->rcItem.top + iconY, hIconExtend, 16, 16, 0, NULL, DI_NORMAL);

			TextOut(hdc, 50, y, text, lstrlen(text));
		}
		else if (lpDraw->CtlID == show_db_def) {
			hdc = lpDraw->hDC;

			bgBrush = CreateSolidBrush(RGB(230, 230, 230));
			FillRect(hdc, &lpDraw->rcItem, bgBrush);
			DeleteObject(bgBrush);

			SetBkMode(hdc, TRANSPARENT);

			HFONT hFont = (HFONT)SendMessage(lpDraw->hwndItem, WM_GETFONT, 0, 0);
			if (hFont) {
				SelectObject(hdc, hFont);
			}

			wchar_t text[32]{ 0 };
			GetWindowText(lpDraw->hwndItem, text, 32);

			SIZE textSize;
			GetTextExtentPoint32(hdc, text, lstrlen(text), &textSize);

			int x = 6;
			int y = (lpDraw->rcItem.bottom - lpDraw->rcItem.top - textSize.cy) / 2;

			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, x, y, text, lstrlen(text));
		}
		else if (lpDraw->CtlID == db_table_info) {
			bgBrush = CreateSolidBrush(RGB(127, 178, 228));
			FillRect(hdc, &rc, bgBrush);
			DeleteObject(bgBrush);
			hPen = CreatePen(PS_SOLID, 4, RGB(30, 120, 255));
			SetTextColor(hdc, RGB(253, 253, 235));

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

			SetBkMode(hdc, TRANSPARENT);

			wchar_t text[32]{ 0 };
			SIZE textSize = { 0 };
			GetTextExtentPoint32(hdc, text, lstrlen(text), &textSize);
			GetWindowText(lpDraw->hwndItem, text, 32);

			TextOut(hdc, 6, (30 - textSize.cy) / 4, text, lstrlen(text));

		}
	}
	else if (msg == WM_SIZE) {
		RECT left_wind;
		GetClientRect(hWnd, &left_wind);

		MoveWindow(hNoDB, left_wind.left, left_wind.bottom / 2 - 60, left_wind.right - 2, 80, false);

		if (active_table_left == 0) {
			for (size_t j = 1, k = 1 / 2; j < db_window.size(); j += 2, k++)
			{
				MoveWindow(db_window[j], 30, 225 + (30 * k) + (k * 2), 30, 30, false);
				MoveWindow(db_window[j - 1], 60, 225 + (30 * k) + (k * 2), left_wind.right - 100, 30, false);

			}
		}
		else {

			int tbls = 1;
			int table_button_position_y = 0;
			for (size_t i = 0; i < db_tables_left.size(); i++)
			{
				table_button_position_y = 225 + (30 * (active_table_left / 2)) + ((active_table_left / 2) * 2) + 30 * tbls;
				MoveWindow(db_tables_left[i], 60, table_button_position_y, left_wind.right - 100, 30, false);
				tbls++;
			}
			for (size_t j = active_table_left + 2, k = active_table_left / 2; j < db_window.size(); j += 2, k++)
			{
				MoveWindow(db_window[j], 30, 225 + (30 * k) + (k * 2) + tbls * 30 + 2, 30, 30, false);
				MoveWindow(db_window[j - 1], 60, 225 + (30 * k) + (k * 2) + tbls * 30 + 2, left_wind.right - 100, 30, false);
			}
			for (size_t i = 1, k = active_table_left / 2; i < active_table_left + 2; i += 2)
			{
				MoveWindow(db_window[i], 30, 225 + (30 * k) + (k * 2), 30, 30, false);
				MoveWindow(db_window[i - 1], 60, 225 + (30 * k) + (k * 2), left_wind.right - 100, 30, false);
			}
		}
	}
	else if (msg == WM_ERASEBKGND) {
		return 1;
	}
	else if (msg == WM_COMMAND) {
		int controlID = LOWORD(wp);
		HWND hwndClicked = (HWND)lp;  // Кнопка, яку натиснули

		if (controlID == btn_show_table_def) {

			for (size_t i = 0; i < db_window.size(); i++) {
				if (db_window[i] == hwndClicked && active_table_left == i) {
					active_table_left = 0;
					ClearLeft(i);
				}
				else if (db_window[i] == hwndClicked && active_table_left != i) {
					active_table_left = i;
					if (prev_table != -1)
						ClearLeft(prev_table);

					prev_table = i;
					ClearLeft(i);

					sqlite3_close(db);
					std::string dbName = "..\\..\\databases/";
					char buffer[24]{};
					GetWindowTextA(db_window[i - 1], buffer, 24);
					dbName += buffer;

					if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
						std::cout << "Open error" << sqlite3_errmsg(db) << std::endl;
					}

					else {
						sqlite3_stmt* stmt;
						const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";
						if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
							std::cout << "prepare errror " << std::endl;
							sqlite3_close(db);
						}
						else {
							int tbls = 1;
							int table_button_position_y;

							RECT left_rec;
							GetClientRect(left_panel, &left_rec);
							while (sqlite3_step(stmt) == SQLITE_ROW) {

								table_button_position_y = 225 + (30 * (i / 2)) + ((i / 2) * 2) + 30 * tbls;
								tbls++;

								const char* tblName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
								HWND Table = CreateWindowExA(0, "STATIC", tblName, WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 60, table_button_position_y, left_rec.right - 100, 30, left_panel, (HMENU)db_table_info, NULL, NULL);
								db_tables_left.push_back(Table);
							}
							for (size_t j = i + 2, k = i / 2; j < db_window.size(); j += 2, k++)
							{
								MoveWindow(db_window[j], 30, 225 + (30 * k) + (k * 2) + tbls * 30 + 2, 30, 30, true);
								MoveWindow(db_window[j - 1], 60, 225 + (30 * k) + (k * 2) + tbls * 30 + 2, left_rec.right - 100, 30, true);

							}
						}
						sqlite3_finalize(stmt);
					}
				}
			}
		}
	}
	else return DefWindowProc(hWnd, msg, wp, lp);
}