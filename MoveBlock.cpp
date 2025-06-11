#include "MoveBlock.h"

void MoveBlock(int index) {
	int db_button_position_y;
	for (size_t i = 0, j = 1; i < databases_name.size(); i++)
	{
		if (i > index && index != -1) {
			db_button_position_y = 35 * i + 25 * table_matrix.size() + (db_tables.size() > 0 ? 40 : 10);
			MoveWindow(databases[i], 10, db_button_position_y, 125, 30, true);
			InvalidateRect(databases[i], NULL, TRUE);
			j++;
		}

	}
}