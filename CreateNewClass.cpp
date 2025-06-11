#include "CreateNewClass.h"

WNDCLASSEX CreateNewClass(HBRUSH BgColor, HCURSOR cursor, HINSTANCE hinst, HICON icon, LPCWSTR Name, WNDPROC procedure) {
	WNDCLASSEX NWC{ 0 };
	NWC.cbSize = sizeof(WNDCLASSEX);
	NWC.hbrBackground = BgColor;
	NWC.hCursor = cursor;
	NWC.hInstance = hinst;
	NWC.hIcon = icon;
	NWC.lpszClassName = Name;
	NWC.lpfnWndProc = procedure;

	return NWC;
}