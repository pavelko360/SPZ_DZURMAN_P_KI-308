#include "PopulateDBList.h"
#include <filesystem>
#include <Windows.h>
#include "Definitions.h"
extern std::vector<std::string> databases_name;
extern HWND hNoDB;
extern std::vector<HWND>db_window;
extern HWND left_panel;
extern HFONT default_font;


void PopulateDBList() {
	databases_name.clear();
	for (const auto& entry : std::filesystem::directory_iterator("..\\..\\databases/")) {
		if (entry.is_regular_file()) {
			std::string name = entry.path().filename().string();
			if (name.ends_with(".sqlite") || name.ends_with(".db") || name.ends_with("sqlite3")) {
				databases_name.push_back(name);
			}
		}
	}

	if (databases_name.empty()) {
		ShowWindow(hNoDB, SW_SHOW);
	}
	else {
		for (HWND hwnd : db_window)
			DestroyWindow(hwnd);
		db_window.clear();


		RECT left_rec;
		GetClientRect(left_panel, &left_rec);

		ShowWindow(hNoDB, SW_HIDE);
		for (size_t i = 0; i < databases_name.size(); i++)
		{
			HWND db = CreateWindowExA(0, "STATIC", databases_name[i].c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 60, 225 + (30 * i) + (i * 2), left_rec.right - 100, 30, left_panel, (HMENU)show_db_def, NULL, NULL);
			SendMessage(db, WM_SETFONT, WPARAM(default_font), TRUE);
			HWND db2 = CreateWindowEx(0, L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 30, 225 + (30 * i) + i * 2, 30, 30, left_panel, (HMENU)btn_show_table_def, NULL, NULL);
			db_window.push_back(db);
			db_window.push_back(db2);
			ShowWindow(db_window[i * 2], SW_SHOW);
		}
	}
}