#include <Windows.h>
#include "Definitions.h"
#include <winsqlite/winsqlite3.h>

#include <vector>
#include <cwchar>
#include <richedit.h>
#include <string>
#include "resource1.h"
//#pragma comment(lib, "Msftedit.lib")
#pragma comment(lib, "winsqlite3.lib")

#include "MoveBlock.h"
#include "GetTables.h"
#include "Get_titles.h"
#include "getTableColumns.h"
#include "GetRowsNum.h"
#include "ClearLeft.h"
#include "Query_result.h"
#include "GetRows.h"
#include "ClearTables.h"
#include "ClearRow.h"
#include "CreateNewClass.h"
#include "PopulateDBList.h"
#include "EditBasesStruct.h"

#include "SoftWareMainProcedure.h"
#include "LeftPanelProcedure.h"
#include "RightPanelProcedure.h"
#include "CustomClassProcedure.h"
#include "QueryManageProcedure.h"
#include "SuccesWindowProcedure.h"
#include "DbManagementProcedure.h"

float left_panel_width = 0.22;
//using std::cout, std::endl;
using std::string;


HWND hNoDB = NULL;
HWND left_panel = NULL, right_panel = NULL;
HWND button_sql = NULL;
HWND button_tbl = NULL;
HWND button_stg = NULL;
HWND button_res = NULL;
HWND button_create = NULL; HWND button_insert = NULL; HWND button_select = NULL; HWND button_drop = NULL;
HWND button_clear = NULL;
HWND inform_window = NULL;
HWND query_field_parent = NULL;
HWND num_field = NULL;
HWND query_field = NULL;
HWND query_manage_field = NULL;
HRGN hRgn = NULL;
HWND btn_run = NULL;
HWND result_window = NULL;
HWND succes_window = NULL;
HWND succes_window_info = NULL;
HWND databases_window = NULL;
HFONT query_font = NULL, inform_font = NULL;
HFONT default_font = NULL , hFont = NULL;
HFONT bold_font = NULL;

HWND settings_wind = NULL;

UINT activeButtonId = btn_tab_sql;
UINT active_db = 0;
UINT active_table_left = 0;
HICON hIconSQL, hIconDBS, hIconStg,hIconStatus, hIconRun, hIconExtend, g_hLogo;

sqlite3* db = NULL;
short pressed_btn = -1;
short active_table = -1;

short prev_table = -1;

short succes = -1;

char inform_text[64]{ 0 };
string buff(256, '\0');

std::vector<HWND>db_window; //for left panel

std::vector<std::vector<HWND>> table_matrix;

std::vector<HWND>databases;   //for right 
std::vector<HWND>db_tables;
std::vector<HWND>db_tables_left;
std::vector<std::string> databases_name;
static int in = 0;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
//int main() {
	LoadLibrary(L"Msftedit.dll");
	
	HICON hMyIcon = (HICON)LoadImage(NULL, L"..\\..\\png\\main_logo.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);

	WNDCLASSEX SoftwareMainClass = CreateNewClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), NULL, LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1)),
		L"MainWindClass", SoftWareMainProcedure);
	SoftwareMainClass.hIconSm = (HICON)LoadImage(NULL, L"..\\..\\png\\main_logo.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	if (!RegisterClassEx(&SoftwareMainClass)) { return -1; }
	MSG SoftMainMessage = { 0 };

	WNDCLASSEX LeftPanelClass = CreateNewClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), NULL, LoadIcon(NULL, IDI_QUESTION),
		L"LeftPanelClass", LeftPanelProcedure);
	if (!RegisterClassEx(&LeftPanelClass)) { return -1; }



	WNDCLASSEX RightPanelClass = CreateNewClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), NULL, LoadIcon(NULL, IDI_QUESTION),
		L"RightPanelClass", RightPanelProcedure);
	RightPanelClass.hbrBackground = CreateSolidBrush(RGB(240, 240, 240)); //колір фону правої панелі  SETTINGS
	if (!RegisterClassEx(&RightPanelClass)) { return -1; }

	HWND hwnd = CreateWindowEx(0, L"MainWindClass", L"MySQLite", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 724, NULL, NULL, NULL, NULL);

	RECT main_window;
	GetClientRect(hwnd, &main_window);
	int height = main_window.bottom - main_window.top;

	left_panel = CreateWindowEx(0, L"LeftPanelClass", NULL, WS_CHILD | WS_VISIBLE, 0, 0, left_panel_width * main_window.right, height, hwnd, (HMENU)left_window_panel, NULL, NULL);
	right_panel = CreateWindowEx(0, L"RightPanelClass", NULL, SS_WHITERECT | WS_CHILD | WS_VISIBLE, left_panel_width * main_window.right, 0, main_window.right - left_panel_width * main_window.right, height, hwnd, (HMENU)right_window_panel, NULL, NULL);


	bold_font = CreateFont(-15, 0, 0, 0, 700, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UIs");

	RECT left_rec;
	GetClientRect(left_panel, &left_rec);
	hNoDB = CreateWindowEx(0, L"static", L"There is no avaliable databases!\n Create you first database", WS_CHILD | WS_VISIBLE | SS_CENTER, left_rec.left, left_rec.bottom / 2 - 60, left_rec.right - 2, 80, left_panel, (HMENU)db_caption, NULL, NULL);
	hFont = CreateFont(17, 0, 0, 0, FW_LIGHT, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UIs");
	SendMessage(hNoDB, WM_SETFONT, WPARAM(hFont), TRUE);

	inform_font = CreateFont(-11, 0, 0, 0, 700, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UIs");
	default_font = CreateFont(-15, 0, 0, 0, 500, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Aerial");
	PopulateDBList();

	RECT right_window;
	GetClientRect(right_panel, &right_window);
	
	int btn_width = (right_window.right - 0) / 4;
	button_sql = CreateWindowEx(0, L"BUTTON", L"SQL", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, btn_width, 45, right_panel, (HMENU)btn_tab_sql, NULL, NULL);
	button_res = CreateWindowEx(0, L"BUTTON", L"result", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, (btn_width + 1), 0, btn_width, 45, right_panel, (HMENU)btn_tab_result, NULL, NULL);
	button_tbl = CreateWindowEx(0, L"BUTTON", L"DATABASES", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, (btn_width * 2) + 2, 0, btn_width, 45, right_panel, (HMENU)btn_tab_tbl, NULL, NULL);
	button_stg = CreateWindowEx(0, L"BUTTON", L"SETTINGS", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, (btn_width * 3) + 3, 0, btn_width, 45, right_panel, (HMENU)btn_tab_stg, NULL, NULL);

	WNDCLASSEX CustomClass = CreateNewClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), NULL, LoadIcon(NULL, IDI_QUESTION), L"CustomClass", CustomClassProcedure);
	if (!RegisterClassEx(&CustomClass)) { return -1; }

	query_field_parent = CreateWindowEx(0, L"CustomClass", NULL, WS_CHILD | WS_VISIBLE, 5, 75, right_window.right - 10, right_window.bottom - 105, right_panel, (HMENU)query_parent_def, NULL, NULL);
	RECT query_window;
	GetClientRect(query_field_parent, &query_window);
	num_field = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | WS_VISIBLE, 20, 30, 70, right_window.bottom / 2, query_field_parent, (HMENU)num_field_def, NULL, NULL);
	query_field = CreateWindowEx(0, MSFTEDIT_CLASS, NULL, ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 90, 30, right_window.right - 120, right_window.bottom / 2, query_field_parent, (HMENU)query_field_def, NULL, NULL);
	SendMessage(query_field, EM_SETEVENTMASK, 0, ENM_CHANGE);

	inform_window  = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | SS_OWNERDRAW, 20, right_window.bottom / 2 + 30, right_window.right - 50, 20, query_field_parent, (HMENU)inform_window_def, NULL, NULL);
	SendMessage(inform_window, WM_SETFONT, (WPARAM)inform_font, TRUE);//
	strncpy_s(inform_text, "Write \"USE DB database_name\" to create database!", sizeof(inform_text));
	ShowWindow(inform_window, SW_SHOW);

	button_create = CreateWindowEx(0, L"BUTTON", L"CREATE", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 20, query_window.bottom - 157, 120, 35, query_field_parent, (HMENU)btn_create_query, NULL, NULL);
	button_insert = CreateWindowEx(0, L"BUTTON", L"INSERT", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 140, query_window.bottom - 157, 120, 35, query_field_parent, (HMENU)btn_insert_query, NULL, NULL);
	button_select = CreateWindowEx(0, L"BUTTON", L"SELECT", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 260, query_window.bottom - 157, 120, 35, query_field_parent, (HMENU)btn_select_query, NULL, NULL);
	button_drop = CreateWindowEx(0, L"BUTTON", L"DROP", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 380, query_window.bottom - 157, 120, 35, query_field_parent, (HMENU)btn_drop_query, NULL, NULL);
	button_clear = CreateWindowEx(0, L"BUTTON", L"clear", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, right_window.right - 110, query_window.bottom - 157, 80, 35, query_field_parent, (HMENU)btn_clear_query, NULL, NULL);

	WNDCLASSEX QueryManageClass = CreateNewClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), NULL, LoadIcon(NULL, IDI_QUESTION), L"QueryManageClass", QueryManageProcedure);
	if (!RegisterClassEx(&QueryManageClass)) { return -1; }
	query_manage_field = CreateWindowEx(0, L"QueryManageClass",NULL,  WS_CHILD | WS_VISIBLE, 2, query_window.bottom - 102, query_window.right - 4, 100, query_field_parent, (HMENU)query_manage_field_def, NULL, NULL);

	btn_run = CreateWindowEx(0, L"BUTTON", L"RUN", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW , query_window.right - 123, 25, 100, 50, query_manage_field, (HMENU)btn_run_def, NULL, NULL);
	DeleteObject(hRgn);

	query_font = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Consolas");
	SendMessage(query_field, WM_SETFONT, (WPARAM)query_font, TRUE);

	WNDCLASSEX SuccesWindowClass = CreateNewClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), NULL, LoadIcon(NULL, IDI_QUESTION), L"SuccesWindowClass", SuccesWindowProcedure);
	if (!RegisterClassEx(&SuccesWindowClass)) { return -1; }

	result_window = CreateWindowEx(0, L"CustomClass", NULL, WS_CHILD , 5, 75, right_window.right - 10, right_window.bottom - 105, right_panel, (HMENU)result_window_def, NULL, NULL);
	
	
	succes_window = CreateWindowEx(0, L"SuccesWindowClass", NULL, WS_CHILD | WS_VISIBLE , 2, 2, query_window.right - 5, 102, result_window, (HMENU)succes_wind_def, NULL, NULL);
	succes_window_info = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 25, 15, query_window.right - 56, 70, succes_window, (HMENU)succes_wind_info_def, NULL, NULL);
	SendMessage(succes_window_info, WM_SETFONT, (WPARAM)query_font, TRUE);

	WNDCLASSEX DBManageClass = CreateNewClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), NULL, LoadIcon(NULL, IDI_QUESTION), L"DBManagement", DbManagementProcedure);
	if (!RegisterClassEx(&DBManageClass)) { return -1; }
	databases_window = CreateWindowEx(0, L"DBManagement", NULL, WS_CHILD, 5, 75, right_window.right - 10, right_window.bottom - 105, right_panel, (HMENU)databases_window_def, NULL, NULL);
	
	settings_wind = CreateWindowEx(0, L"CustomClass", NULL, WS_CHILD, 5, 75, right_window.right - 10, right_window.bottom - 105, right_panel, (HMENU)settings_wind_def, NULL, NULL);
	HWND lang_stg = CreateWindowEx(0, L"STATIC", L"Language", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 50, 40, 120, 40, settings_wind, (HMENU)option, NULL, NULL);
	HWND lang_btn = CreateWindowEx(0, L"STATIC", L"English", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 220, 40, 120, 40, settings_wind, (HMENU)setted, NULL, NULL);
	HWND theme_stg = CreateWindowEx(0, L"STATIC", L"Theme", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 50, 85, 120, 40, settings_wind, (HMENU)option, NULL, NULL);
	HWND theme_btn = CreateWindowEx(0, L"STATIC", L"Default", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 220, 85, 120, 40, settings_wind, (HMENU)setted, NULL, NULL);
	HWND exten_sth = CreateWindowEx(0, L"STATIC", L"DB extension", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 50, 130, 120, 40, settings_wind, (HMENU)option, NULL, NULL);
	HWND etten_btn = CreateWindowEx(0, L"STATIC", L".db", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 220, 130, 120, 40, settings_wind, (HMENU)setted, NULL, NULL);

	while (GetMessage(&SoftMainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&SoftMainMessage);
		DispatchMessage(&SoftMainMessage);
	}

	return 0;
}

