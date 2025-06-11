#pragma once
#include <Windows.h>
WNDCLASSEX CreateNewClass(HBRUSH BgColor, HCURSOR cursor, HINSTANCE hinst, HICON icon, LPCWSTR Name, WNDPROC procedure);