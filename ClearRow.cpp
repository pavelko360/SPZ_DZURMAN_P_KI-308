#include "ClearRow.h"
#include <Windows.h>
#include "MoveBlock.h"
extern std::vector<std::vector<HWND>> table_matrix;
extern short pressed_btn;

void ClearRow() {
	for (size_t i = 0; i < table_matrix.size(); i++)
	{
		for (size_t j = 0; j < table_matrix[i].size(); j++)
		{
			DestroyWindow(table_matrix[i][j]);
		}
		table_matrix[i].clear();
	}
	table_matrix.clear();

	MoveBlock(pressed_btn);
}