#include "SuccesWindowProcedure.h"
#include "Definitions.h"
#include <winsqlite/winsqlite3.h>
#include <string>
extern short succes;
extern sqlite3* db;
extern std::string buff;

LRESULT CALLBACK SuccesWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT rc;
		GetClientRect(hWnd, &rc);

		HBRUSH bgBrush;

		if (succes == 1) {
			bgBrush = CreateSolidBrush(RGB(165, 230, 155)); // фон успіху
			FillRect(hdc, &rc, bgBrush);
			DeleteObject(bgBrush);
		}
		else if (succes == 0) {
			bgBrush = CreateSolidBrush(RGB(230, 145, 145)); // фон помилки
			FillRect(hdc, &rc, bgBrush);
			DeleteObject(bgBrush);
		}
		else {
			bgBrush = CreateSolidBrush(RGB(200, 200, 200)); // нейтральний фон
			FillRect(hdc, &rc, bgBrush);
			DeleteObject(bgBrush);
		}

		// Нижня полоска
		HBRUSH hBrush = CreateSolidBrush(RGB(180, 180, 180));
		RECT bottom_border = { rc.left, rc.bottom - 2, rc.right, rc.bottom };
		FillRect(hdc, &bottom_border, hBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
	}
	else if (msg == WM_DRAWITEM) {
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lp;
		HDC hdc = pDIS->hDC;
		RECT rc = pDIS->rcItem;
		if (pDIS->CtlID == succes_wind_info_def) {

			HBRUSH bgBrush;
			char text[256]{ 0 };

			if (succes == 1) {
				bgBrush = CreateSolidBrush(RGB(165, 230, 155)); // фон
				FillRect(hdc, &rc, bgBrush);
				DeleteObject(bgBrush);
				strcat_s(text, "Query completed succesfully!\n");
				strcat_s(text, "\n-");
				strcat_s(text, buff.c_str());
			}
			else if (succes == 0) {
				bgBrush = CreateSolidBrush(RGB(230, 145, 145)); // фон
				FillRect(hdc, &rc, bgBrush);
				strcat_s(text, sqlite3_errmsg(db));
				strcat_s(text, "\n-");
				strcat_s(text, buff.c_str());
			}
			else {
				bgBrush = CreateSolidBrush(RGB(200, 200, 200)); // фон
				FillRect(hdc, &rc, bgBrush);
				strcat_s(text, "There will be result of query execute!");
			}
			DeleteObject(bgBrush);

			SetBkMode(hdc, TRANSPARENT); // Прозорий фон
			SetTextColor(hdc, RGB(0, 0, 0));

			DrawTextA(hdc, text, -1, &rc, DT_WORDBREAK | DT_LEFT | DT_TOP);
		}

	}
	else return DefWindowProc(hWnd, msg, wp, lp);
}