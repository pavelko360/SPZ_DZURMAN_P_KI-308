#include "EditBasesStruct.h"
#include <Windows.h>
#include <vector>
#include "Definitions.h"
#include <string>
extern HFONT default_font;
extern std::vector<std::string> databases_name;
extern std::vector<HWND>databases;
extern HWND databases_window;

void EditBasesStruct() {
	if (!databases_name.empty()) {

		for (HWND hwnd : databases)
			DestroyWindow(hwnd);
		databases.clear();

		for (size_t i = 0; i < databases_name.size(); i++)
		{
			HWND db = CreateWindowExA(0, "BUTTON", databases_name[i].c_str(), WS_CHILD | WS_VISIBLE | BS_LEFT | BS_OWNERDRAW, 10, 35 * i + 10, 125, 30, databases_window, (HMENU)btn_db_manage_def, NULL, NULL);
			SendMessage(db, WM_SETFONT, WPARAM(default_font), TRUE);
			databases.push_back(db);

		}
	}
}