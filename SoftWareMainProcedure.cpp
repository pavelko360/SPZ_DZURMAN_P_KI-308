#include "SoftWareMainProcedure.h"
#include "Definitions.h"

extern float left_panel_width;

LRESULT CALLBACK SoftWareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

	HWND panel_l = GetDlgItem(hWnd, left_window_panel);
	HWND panel_r = GetDlgItem(hWnd, right_window_panel);

	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (msg == WM_SIZE) {
		RECT main_wind;
		GetClientRect(hWnd, &main_wind);
		int height = main_wind.bottom - main_wind.top;

		MoveWindow(panel_l, 0, 0, left_panel_width * main_wind.right, height, false);
		MoveWindow(panel_r, left_panel_width * main_wind.right, 0, main_wind.right - left_panel_width * main_wind.right, height, false);

		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	}
	else if (msg == WM_GETMINMAXINFO) {
		MINMAXINFO* minMax = (MINMAXINFO*)lp;
		minMax->ptMinTrackSize.x = 1024;
		minMax->ptMinTrackSize.y = 724;
	}

	else return DefWindowProc(hWnd, msg, wp, lp);

}