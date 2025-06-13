#include "QueryManageProcedure.h"
#include "Definitions.h"
#include <string>
#include "PopulateDBList.h"
#include "ClearRow.h"
#include "Get_titles.h"
#include "Query_result.h"
extern HWND query_field_parent, inform_window, result_window;
extern HICON hIconRun;
extern HFONT default_font;
extern sqlite3* db;
extern std::string buff;
extern char inform_text[64];
extern UINT activeButtonId;
extern short succes;

LRESULT CALLBACK QueryManageProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT rc;
		GetClientRect(hWnd, &rc);

		HBRUSH bgBrush = CreateSolidBrush(RGB(190, 200, 210)); //фон 
		FillRect(hdc, &rc, bgBrush);
		DeleteObject(bgBrush);

		HBRUSH hBrush = CreateSolidBrush(RGB(180, 180, 180)); //колір полоски
		RECT top_border = { 0, 0, rc.right, rc.top + 2 };
		FillRect(hdc, &top_border, hBrush);
		DeleteObject(hBrush);
	}
	else if (msg == WM_SIZE) {
		RECT wind;
		GetClientRect(hWnd, &wind);
		HRGN hRgn = CreateRoundRectRgn(0, 0, wind.right, wind.bottom, 9, 9);
		HRGN topRect = CreateRectRgn(0, 0, wind.right - 1, wind.bottom / 2);
		CombineRgn(hRgn, hRgn, topRect, RGN_OR);
		SetWindowRgn(hWnd, hRgn, TRUE);
		DeleteObject(hRgn);
		DeleteObject(topRect);
		InvalidateRect(query_field_parent, NULL, TRUE);
	}
	else if (msg == WM_DRAWITEM) {
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lp;

		if (pDIS->CtlID == btn_run_def) // ID кнопки
		{


			HDC hdc = pDIS->hDC;
			HRGN hRgn = CreateRectRgnIndirect(&pDIS->rcItem);
			BOOL pressed = (pDIS->itemState & ODS_SELECTED);
			// Заливка кнопки
			HBRUSH hBrush = pressed ? CreateSolidBrush(RGB(30, 120, 255)) : CreateSolidBrush(RGB(230, 230, 230)); // колір кнопки
			FillRgn(hdc, hRgn, hBrush);
			DeleteObject(hBrush);
			DeleteObject(hRgn);

			HPEN pen = CreatePen(PS_SOLID, 2, RGB(180, 180, 180)); // рамка
			HGDIOBJ oldPen = SelectObject(hdc, pen);
			HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
			RoundRect(hdc, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 0, 0);
			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);
			DeleteObject(pen);

			int iconSize = 20;
			int spacing = 12; // відступ між іконкою і текстом

			SIZE textSize = { 0 };
			GetTextExtentPoint32(hdc, L"RUN", 3, &textSize);

			int totalWidth = iconSize + spacing + textSize.cx;

			int contentStartX = pDIS->rcItem.left + ((pDIS->rcItem.right - pDIS->rcItem.left) - totalWidth) / 2;
			int iconX = contentStartX;
			int iconY = pDIS->rcItem.top + ((pDIS->rcItem.bottom - pDIS->rcItem.top) - iconSize) / 2;

			int textX = iconX + iconSize + spacing;
			int textY = pDIS->rcItem.top + ((pDIS->rcItem.bottom - pDIS->rcItem.top) - textSize.cy) / 2;

			if (pressed)
				hIconRun = (HICON)LoadImage(GetModuleHandle(NULL), L"..\\..\\png\\pressed_run.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);
			else hIconRun = (HICON)LoadImage(GetModuleHandle(NULL), L"..\\..\\png\\button_run.ico", IMAGE_ICON, 20, 20, LR_LOADFROMFILE);

			if (hIconRun)
				DrawIconEx(hdc, iconX, iconY, hIconRun, iconSize, iconSize, 0, NULL, DI_NORMAL);

			SelectObject(hdc, default_font);
			SetTextColor(hdc, pressed ? RGB(255, 255, 255) : RGB(30, 120, 255));
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, textX, textY, L"RUN", 3);
		}

	}
	else if (msg == WM_COMMAND) {
		if (LOWORD(wp) == btn_run_def) {

			std::string bdName;
			std::string sql_query;
			if (buff.compare(0, 7, "USE DB ") == 0 || buff.compare(0, 7, "use db ") == 0) {
				sqlite3_close(db);
				for (size_t i = 7; i <= buff.size() - 2; i++)
				{
					if (buff[i] == ' ' || buff[i] == '\t' || buff[i] == '\n')
						continue;
					bdName.push_back(buff[i]);
				}
				bdName = "..\\..\\databases/" + bdName + ".db";

				//std::cout << buff << std::endl;
				std::fill(buff.begin(), buff.end(), '\0');
				//std::cout << buff << std::endl;

				int rc = sqlite3_open(bdName.c_str(), &db);
				if (rc != SQLITE_OK) {
					strncpy_s(inform_text, "Open DataBase error!", sizeof(inform_text));
					ShowWindow(inform_window, SW_HIDE);
					ShowWindow(inform_window, SW_SHOW);
				}
				else {
					//std::cout << "DB opened" << bdName.c_str() << std::endl;
					strncpy_s(inform_text, "SUCCES!", sizeof(inform_text));
					ShowWindow(inform_window, SW_HIDE);
					ShowWindow(inform_window, SW_SHOW);
				}

				PopulateDBList();
			}
			else if (buff.compare(0, 10, "DELETE DB ") == 0 || buff.compare(0, 10, "delete db ") == 0) {
				sqlite3_close(db);
				for (size_t i = 10; i <= buff.size() - 2; i++)
				{
					if (buff[i] == ' ' || buff[i] == '\t' || buff[i] == '\n')
						continue;
					bdName.push_back(buff[i]);
					//std::cout << buff << "---" << bdName << std::endl;
				}
				bdName = "..\\..\\databases/" + bdName + ".db";

				//std::cout << buff << std::endl;
				std::fill(buff.begin(), buff.end(), '\0');
				//std::cout << buff << std::endl;

				if (std::remove(bdName.c_str()) == 0) {
					strncpy_s(inform_text, "SUCCES!", sizeof(inform_text));
					ShowWindow(inform_window, SW_HIDE);
					ShowWindow(inform_window, SW_SHOW);
				}
				else {
					strncpy_s(inform_text, "Error!", sizeof(inform_text));
					ShowWindow(inform_window, SW_HIDE);
					ShowWindow(inform_window, SW_SHOW);
				}

				PopulateDBList();
			}
			else {
				if (buff[0] != 0) {
					sql_query = buff;
					ClearRow();

					sqlite3_stmt* stmt = nullptr;
					if (sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
						int step_rc = sqlite3_step(stmt);
						if (step_rc == SQLITE_DONE) {
							sqlite3_finalize(stmt);
							UINT prev_btn = activeButtonId;
							succes = 1;
							ShowWindow(query_field_parent, SW_HIDE);
							activeButtonId = btn_tab_result;
							InvalidateRect(GetDlgItem(hWnd, 12), NULL, TRUE);
							InvalidateRect(GetDlgItem(hWnd, prev_btn), NULL, TRUE);
							ShowWindow(result_window, SW_SHOW);
						}
						else if (step_rc == SQLITE_ROW) {
							int hight_index = 0;
							Get_titles(stmt, hight_index);
							hight_index++;
							Query_result(stmt, hight_index);

							while (sqlite3_step(stmt) == SQLITE_ROW) {
								hight_index++;
								Query_result(stmt, hight_index);

							}
							UINT prev_btn = activeButtonId;
							succes = 1;
							ShowWindow(query_field_parent, SW_HIDE);
							activeButtonId = btn_tab_result;
							InvalidateRect(GetDlgItem(hWnd, 12), NULL, TRUE);
							InvalidateRect(GetDlgItem(hWnd, prev_btn), NULL, TRUE);
							ShowWindow(result_window, SW_SHOW);
							sqlite3_finalize(stmt);
						}
						else {
							//std::cout << "sqlite3_step error!" << sqlite3_errmsg(db) << std::endl;
							MessageBoxA(NULL, "SQLite step error!", "Error", MB_OK);
							sqlite3_finalize(stmt);
						}
					}
					else {
						//std::cout << sqlite3_errmsg(db) << std::endl;
						UINT prev_btn = activeButtonId;
						succes = 0;
						ShowWindow(query_field_parent, SW_HIDE);
						activeButtonId = btn_tab_result;
						InvalidateRect(GetDlgItem(hWnd, 12), NULL, TRUE);
						InvalidateRect(GetDlgItem(hWnd, prev_btn), NULL, TRUE);
						ShowWindow(result_window, SW_SHOW);

					}
				}
				//else {
				//	//std::cout << "buff is empty" << std::endl;
				//	MessageBoxA(NULL, "Buff is empty!", "Error", MB_OK);
				//}
			}
		}
	}
	else return DefWindowProc(hWnd, msg, wp, lp);
}