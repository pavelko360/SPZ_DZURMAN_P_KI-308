#include "CustomClassProcedure.h"
#include "Definitions.h"
#include <string>
#include<vector>

extern HFONT default_font, inform_font;
extern char inform_text[64];
extern HWND query_field_parent, result_window, inform_window, query_field;
extern std::string buff;

LRESULT CALLBACK CustomClassProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	RECT wind;
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

		BOOL pressed = (lpDraw->itemState & ODS_SELECTED);

		// Фон
		HBRUSH bgBrush = pressed ? CreateSolidBrush(RGB(230, 230, 230)) : CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hdc, &rc, bgBrush);
		DeleteObject(bgBrush);

		HPEN pen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));		//рамка 
		HPEN oldPen = (HPEN)SelectObject(hdc, pen);
		MoveToEx(hdc, rc.left, rc.top, NULL); LineTo(hdc, rc.right, rc.top);
		MoveToEx(hdc, rc.left, rc.bottom - 1, NULL); LineTo(hdc, rc.right, rc.bottom - 1);
		MoveToEx(hdc, rc.left, rc.top, NULL); LineTo(hdc, rc.left, rc.bottom);
		MoveToEx(hdc, rc.right - 1, rc.top, NULL); LineTo(hdc, rc.right - 1, rc.bottom);
		SelectObject(hdc, oldPen);
		DeleteObject(pen);

		SelectObject(hdc, default_font);
		SetTextColor(hdc, pressed ? RGB(30, 120, 255) : id == btn_clear_query ? RGB(50, 100, 135) : RGB(145, 75, 155));
		SetBkMode(hdc, TRANSPARENT);

		LPCWSTR text = L"";
		HICON hIcon = NULL;

		if (id == btn_create_query) {
			text = L"CREATE";
		}
		else if (id == btn_insert_query) {
			text = L"INSERT";
		}
		else if (id == btn_select_query) {
			text = L"SELECT";
		}
		else if (id == btn_drop_query) {
			text = L"DROP";
		}
		else if (id == btn_clear_query) {
			text = L"Clear";
		}

		DrawText(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		if (lpDraw->CtlID == inform_window_def) {
			HDC hdc2 = lpDraw->hDC;
			RECT rc2 = lpDraw->rcItem;

			HBRUSH hBrush = NULL;
			if (inform_text[0] == 'S') {
				hBrush = CreateSolidBrush(RGB(165, 230, 155));
			}
			else hBrush = CreateSolidBrush(RGB(253, 253, 235));

			FillRect(hdc2, &rc2, hBrush);
			DeleteObject(hBrush);

			SetTextColor(hdc2, RGB(0, 0, 0));
			SetBkMode(hdc2, TRANSPARENT);

			SelectObject(lpDraw->hDC, inform_font);

			DrawTextA(lpDraw->hDC, inform_text, -1, &lpDraw->rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		}
		else if (lpDraw->CtlID == query_result_row_def) {
			SelectObject(hdc, default_font);
			HBRUSH hBrushBackground = CreateSolidBrush(RGB(240, 240, 240));
			FillRect(hdc, &rc, hBrushBackground);
			DeleteObject(hBrushBackground);

			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(140, 140, 140));
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);


			char text[32];
			GetWindowTextA(lpDraw->hwndItem, text, sizeof(text));
			SelectObject(lpDraw->hDC, default_font);
			SetBkMode(lpDraw->hDC, TRANSPARENT);
			SetTextColor(lpDraw->hDC, RGB(0, 0, 0));
			lpDraw->rcItem.left += 6;
			DrawTextA(lpDraw->hDC, text, -1, &lpDraw->rcItem, DT_VCENTER | DT_SINGLELINE);
		}
		else if (lpDraw->CtlID == option) {
			SelectObject(hdc, default_font);
			HBRUSH hBrushBackground = CreateSolidBrush(RGB(220, 220, 220));
			FillRect(hdc, &rc, hBrushBackground);
			DeleteObject(hBrushBackground);

			char text[32];
			GetWindowTextA(lpDraw->hwndItem, text, sizeof(text));
			SelectObject(lpDraw->hDC, default_font);
			SetBkMode(lpDraw->hDC, TRANSPARENT);
			SetTextColor(lpDraw->hDC, RGB(0, 0, 0));
			lpDraw->rcItem.left += 6;
			strncat_s(text, ":", 32);
			DrawTextA(lpDraw->hDC, text, -1, &lpDraw->rcItem, DT_VCENTER | DT_SINGLELINE);
		}
		else if (lpDraw->CtlID == setted) {
			SelectObject(hdc, default_font);
			HBRUSH hBrushBackground = CreateSolidBrush(RGB(200, 200, 200));
			FillRect(hdc, &rc, hBrushBackground);
			DeleteObject(hBrushBackground);

			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);

			char text[32];
			GetWindowTextA(lpDraw->hwndItem, text, sizeof(text));
			SelectObject(lpDraw->hDC, default_font);
			SetBkMode(lpDraw->hDC, TRANSPARENT);
			SetTextColor(lpDraw->hDC, RGB(0, 0, 0));
			lpDraw->rcItem.left += 6;
			DrawTextA(lpDraw->hDC, text, -1, &lpDraw->rcItem, DT_VCENTER | DT_SINGLELINE);
		}
	}
	else if (msg == WM_SIZE) {
		GetClientRect(hWnd, &wind);
		HRGN hRgn = CreateRoundRectRgn(0, 0, wind.right, wind.bottom, 9, 9);
		SetWindowRgn(hWnd, hRgn, TRUE);
		DeleteObject(hRgn);
		InvalidateRect(query_field_parent, NULL, TRUE);
		InvalidateRect(result_window, NULL, TRUE);
	}
	else if (msg == WM_COMMAND) {

		if (LOWORD(wp) == query_field_def && HIWORD(wp) == EN_CHANGE) {
			ShowWindow(inform_window, SW_HIDE);
			std::string key(10, 0);
			std::vector<std::string> key_words = { "CREATE" , "SELECT", "DELETE" };
			size_t word_len = 0;

			int len = GetWindowTextA((HWND)lp, &buff[0], 256);
			buff.resize(len + 1);
			for (size_t i = 0; i < buff.size(); i++)
			{
				if (buff[i] == ' ' || buff[i] == '\t' || buff[i] == '\n') {
					std::fill(key.begin(), key.end(), '\0');
					word_len = 0;
					continue;
				}
				if (buff[i] == '\0')
					break;
				if (word_len <= key.size()) {
					key[word_len] = buff[i];
					word_len++;
				}

			}
			key.resize(word_len);

		}
		else if (LOWORD(wp) == btn_clear_query) {
			SetWindowText(query_field, L"");
			ShowWindow(inform_window, SW_HIDE);
		}
		else if (LOWORD(wp) == btn_create_query) {
			SetWindowText(query_field, L"CREATE TABLE table_name (\nfield1_name type [PRIMARY KEY] [AUTOINCREMENT] [NOT NULL] [UNIQUE] [DEFAULT value],\nfield2_name type [...],\n...\n)");
			strncpy_s(inform_text, "Write \"USE DB database_name\" to create database!", sizeof(inform_text));
			ShowWindow(inform_window, SW_HIDE);
			ShowWindow(inform_window, SW_SHOW);

		}
		else if (LOWORD(wp) == btn_insert_query) {
			SetWindowText(query_field, L"INSERT INTO table_name (field1_name, field2_name, ...)\nVALUES (value1, value2, ...);");
			ShowWindow(inform_window, SW_HIDE);
		}
		else if (LOWORD(wp) == btn_select_query) {
			SetWindowText(query_field, L"SELECT field1_name, field2_name, ... FROM tbl_name;");
			strncpy_s(inform_text, "Use \'*\' to choose all fields!", sizeof(inform_text));
			ShowWindow(inform_window, SW_HIDE);
			ShowWindow(inform_window, SW_SHOW);

		}
		else if (LOWORD(wp) == btn_drop_query) {
			SetWindowText(query_field, L"DROP TABLE table_name;");
			strncpy_s(inform_text, "Write \"DELETE DB database_name\" to delete database!", sizeof(inform_text));
			ShowWindow(inform_window, SW_HIDE);
			ShowWindow(inform_window, SW_SHOW);

		}
	}
	else return DefWindowProc(hWnd, msg, wp, lp);
}