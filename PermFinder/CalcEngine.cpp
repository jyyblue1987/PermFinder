#include "stdafx.h"
#include "CalcEngine.h"
#include <vector>
using namespace std;

int** parseInputData(CString &data, int &row, int &col)
{
	vector<CString> v;

	int nTokenPos = 0;
	CString strToken = data.Tokenize(_T("\r\n"), nTokenPos);


	v.push_back(strToken);

	while(true)
	{
		CString strToken = data.Tokenize(_T("\r\n"), nTokenPos);
		if( strToken.IsEmpty() )
			break;

		v.push_back(strToken);
	}
	
	row = (int)v.size();
	if( row > 0 )
		col = v[0].GetLength();
	else
		col = 0;



	return NULL;
}