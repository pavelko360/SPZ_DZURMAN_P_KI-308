#include "RightPanelProcedure.h"
#include "Definitions.h"
#include <iostream>
#include "EditBasesStruct.h"
#include <vector>
extern UINT activeButtonId;
extern HFONT default_font;
extern HICON hIconSQL, hIconDBS, hIconStg, hIconStatus;
extern HWND query_field_parent, num_field, query_field, result_window, databases_window, query_manage_field, btn_run, inform_window;
extern HWND button_create, button_insert, button_insert, button_select, button_drop, button_clear, succes_window, succes_window_info, settings_wind;
extern HWND button_sql, button_res, button_tbl, button_stg;
extern std::vector<std::vector<HWND>> table_matrix;
extern short pressed_btn, active_table;
extern std::vector<HWND>db_tables;

LRESULT CALLBACK RightPanelProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	if (msg == WM_DRAWITEM)
	{

		LPDRAWITEMSTRUCT lpDraw = (LPDRAWITEMSTRUCT)lp;
		HDC hdc = lpDraw->hDC;
		RECT rc = lpDraw->rcItem;
		UINT id = lpDraw->CtlID;

		// Фон
		HBRUSH bgBrush = (id == activeButtonId) ? CreateSolidBrush(RGB(240, 240, 240)) : CreateSolidBrush(RGB(220, 220, 220));
		FillRect(hdc, &rc, bgBrush);
		DeleteObject(bgBrush);

		// Обводка (для неактивних кнопок)
		if (id != activeButtonId) {
			HPEN pen = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
			HPEN oldPen = (HPEN)SelectObject(hdc, pen);
			MoveToEx(hdc, rc.left, rc.bottom - 1, NULL); LineTo(hdc, rc.right, rc.bottom - 1);
			MoveToEx(hdc, rc.left, rc.top, NULL); LineTo(hdc, rc.left, rc.bottom);
			MoveToEx(hdc, rc.right - 1, rc.top, NULL); LineTo(hdc, rc.right - 1, rc.bottom);
			SelectObject(hdc, oldPen);
			DeleteObject(pen);
		}

		// Текст і іконка
		SelectObject(hdc, default_font);
		SetTextColor(hdc, RGB(30, 120, 255));
		SetBkMode(hdc, TRANSPARENT);

		LPCWSTR text = L"";
		HICON hIcon = NULL;

		if (id == btn_tab_sql) {
			text = L"SQL";
			hIcon = hIconSQL;
		}
		else if (id == btn_tab_tbl) {
			text = L"DATABASES";
			hIcon = hIconDBS;
		}
		else if (id == btn_tab_stg) {
			text = L"SETTINGS";
			hIcon = hIconStg;
		}
		else if (id == btn_tab_result) {
			text = L"STATUS";
			hIcon = hIconStatus;
		}

		int iconSize = 20;
		int spacing = 8; // відступ між іконкою і текстом

		SIZE textSize = { 0 };
		GetTextExtentPoint32(hdc, text, lstrlen(text), &textSize);

		int totalWidth = iconSize + spacing + textSize.cx;

		int contentStartX = rc.left + ((rc.right - rc.left) - totalWidth) / 2;
		int iconX = contentStartX;
		int iconY = rc.top + ((rc.bottom - rc.top) - iconSize) / 2;

		int textX = iconX + iconSize + spacing;
		int textY = rc.top + ((rc.bottom - rc.top) - textSize.cy) / 2;

		// Малюємо іконку, якщо є
		if (hIcon)
			DrawIconEx(hdc, iconX, iconY, hIcon, iconSize, iconSize, 0, NULL, DI_NORMAL);

		// Малюємо текст
		TextOut(hdc, textX, textY, text, lstrlen(text));

	}
	else if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 240));
		FillRect(hdc, &ps.rcPaint, hBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
		return 0;
	}
	else if (msg == WM_CREATE) {
		hIconSQL = (HICON)LoadImage(GetModuleHandle(NULL), L"..\\..\\png\\list.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
		if (!hIconSQL) {
			MessageBox(NULL, L"Не вдалося завантажити іконку! sql", L"Помилка", MB_OK | MB_ICONERROR);
		}
		hIconDBS = (HICON)LoadImage(GetModuleHandle(NULL), L"..\\..\\png\\dbs2.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
		if (!hIconDBS) {
			MessageBox(NULL, L"Не вдалося завантажити іконку! databases", L"Помилка", MB_OK | MB_ICONERROR);
		}
		hIconStatus = (HICON)LoadImage(GetModuleHandle(NULL), L"..\\..\\png\\btn_status.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
		if (!hIconStatus) {
			MessageBox(NULL, L"Не вдалося завантажити іконку! status", L"Помилка", MB_OK | MB_ICONERROR);
		}
		hIconStg = (HICON)LoadImage(GetModuleHandle(NULL), L"..\\..\\png\\stg.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
		if (!hIconStg) {
			MessageBox(NULL, L"Не вдалося завантажити іконку! settings", L"Помилка", MB_OK | MB_ICONERROR);
		}

	}
	else if (msg == WM_SIZE) {
		RECT right_wind;
		GetClientRect(hWnd, &right_wind);

		MoveWindow(query_field_parent, 5, 75, right_wind.right - 10, right_wind.bottom - 105, false);
		MoveWindow(num_field, 20, 30, 70, right_wind.bottom / 2, false);
		MoveWindow(query_field, 90, 30, right_wind.right - 120, right_wind.bottom / 2, false);

		MoveWindow(result_window, 5, 75, right_wind.right - 10, right_wind.bottom - 105, false);
		MoveWindow(databases_window, 5, 75, right_wind.right - 10, right_wind.bottom - 105, false);

		RECT query_window;
		GetClientRect(query_field_parent, &query_window);
		MoveWindow(query_manage_field, 2, query_window.bottom - 102, query_window.right - 4, 100, false);
		MoveWindow(btn_run, query_window.right - 123, 25, 100, 50, false);


		int btn_width = (right_wind.right - 0) / 4;
		MoveWindow(button_sql, 0, 0, btn_width, 45, false);
		MoveWindow(button_res, btn_width + 1, 0, btn_width, 45, false);
		MoveWindow(button_tbl, btn_width * 2 + 2, 0, btn_width, 45, false);
		MoveWindow(button_stg, btn_width * 3 + 3, 0, btn_width, 45, false);

		MoveWindow(inform_window, 20, right_wind.bottom / 2 + 30, right_wind.right - 50, 20, false);

		MoveWindow(button_create, 20, query_window.bottom - 157, 120, 35, false);
		MoveWindow(button_insert, 140, query_window.bottom - 157, 120, 35, false);
		MoveWindow(button_select, 260, query_window.bottom - 157, 120, 35, false);
		MoveWindow(button_drop, 380, query_window.bottom - 157, 120, 35, false);
		MoveWindow(button_clear, right_wind.right - 110, query_window.bottom - 157, 80, 35, false);


		MoveWindow(succes_window, 2, 2, query_window.right - 5, 102, false);
		MoveWindow(succes_window_info, 25, 15, query_window.right - 56, 70, false);

		MoveWindow(settings_wind, 5, 75, right_wind.right - 10, right_wind.bottom - 105, false);

		if (!table_matrix.empty() && activeButtonId == btn_tab_tbl) {
			for (size_t j = 0; j < table_matrix[0].size(); j++) {
				MoveWindow(table_matrix[0][j], 25 + ((query_window.right - 50) / table_matrix[0].size()) * j, (pressed_btn + 1) * 30 + 40 + 5 * pressed_btn, (query_window.right - 50) / table_matrix[0].size(), 25, false);
			}
			for (size_t i = 1; i < table_matrix.size(); i++)
			{
				for (size_t j = 0; j < table_matrix[0].size(); j++)
				{
					MoveWindow(table_matrix[i][j], 25 + ((query_window.right - 50) / table_matrix[0].size()) * j, (pressed_btn + 1) * 30 + 40 + (5 * pressed_btn) + 25 * i, (query_window.right - 50) / table_matrix[0].size(), 25, false);

				}

			}
		}
		else if (!table_matrix.empty() && activeButtonId == btn_tab_result) {
			RECT parent_window;
			GetClientRect(databases_window, &parent_window);
			int hight_index = 0;
			for (size_t j = 0; j < table_matrix[0].size(); j++) {
				MoveWindow(table_matrix[0][j], 2 + (((parent_window.right - 4) / table_matrix[0].size() * j)), 104 + (25 * hight_index), (parent_window.right - 4) / table_matrix[0].size(), 25, false);
			}
			std::cout << table_matrix[1].size() << std::endl;
			std::cout << table_matrix.size() << std::endl;
			hight_index++;
			for (size_t i = 1; i < table_matrix.size(); i++)
			{
				for (size_t j = 0; j < table_matrix[0].size(); j++)
				{
					MoveWindow(table_matrix[i][j], 2 + (((parent_window.right - 4) / table_matrix[0].size() * j)), 104 + (25 * hight_index), (parent_window.right - 4) / table_matrix[0].size(), 25, false);

				}
				hight_index++;
			}
		}
	}
	else if (msg == WM_ERASEBKGND) {
		return 1;
	}
	else if (msg == WM_COMMAND) {
		UINT prev_btn = activeButtonId;
		for (size_t i = 11; i < 15; i++)	//i = 11, 12 , 13 ,14 - кнопки
		{

			ShowWindow(GetDlgItem(hWnd, activeButtonId + 100), SW_HIDE);
			activeButtonId = LOWORD(wp);
			InvalidateRect(GetDlgItem(hWnd, i), NULL, TRUE);
			InvalidateRect(GetDlgItem(hWnd, prev_btn), NULL, TRUE);
			ShowWindow(GetDlgItem(hWnd, activeButtonId + 100), SW_SHOW);

		}

		if (LOWORD(wp) == btn_tab_tbl) {
			for (size_t j = 0; j < db_tables.size(); j++) {
				DestroyWindow(db_tables[j]);
			}
			active_table = -1;
			pressed_btn = -1;
			EditBasesStruct();
		}

	}
	else return DefWindowProc(hWnd, msg, wp, lp);
}