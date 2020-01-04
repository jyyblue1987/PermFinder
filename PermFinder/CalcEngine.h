#pragma once

#define MAX_PERM_COUNT		1000

BYTE** parseInputData(CString &data, int &row, int &col);
CString calcPath(BYTE **x, int row, int col, int upto);