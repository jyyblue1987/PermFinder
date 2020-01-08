#pragma once

#define MAX_PERM_COUNT		1000

BYTE** parseInputData(CString &data, int &row, int &col);
CString calcPath(BYTE **x, int row, int col, int upto);
CString calcPathTesting(BYTE **x, int row, int col, int start_row, int upto);
CString calcPathWithCompact(BYTE **x, int row, int col, int upto);
CString calcPathMaxLenTotal(BYTE **x, int row, int col, int start_row, int upto);
