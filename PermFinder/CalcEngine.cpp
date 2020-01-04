#include "stdafx.h"
#include "CalcEngine.h"
#include <vector>
using namespace std;


BYTE** parseInputData(CString &data, int &row, int &col)
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

	BYTE **x = (BYTE **) calloc(row, sizeof(BYTE *));
	if( x == NULL )
		return x;

	for(int i = 0; i < row; i++ )
	{
		x[i] = (BYTE *) calloc(col, sizeof(BYTE));
		char *val = v[i].GetBuffer(0);
		for(int j = 0; j < col; j++)
		{
			x[i][j] = val[j] - 48;
		}
	}

	return x;
}

void generatePermList(BYTE *perm_list, int level, int &count, int perm_len, int max_number)
{
	if( level == perm_len )
	{
		count++;
		return;
	}

	int i = 0;
	for(i = 1; i <= max_number; i++)
	{
		if( i > 1 )
			memcpy(perm_list + perm_len * count,  perm_list + perm_len * (count - 1), sizeof(BYTE) * level);

		perm_list[perm_len * count + level] = i;

		generatePermList(perm_list, level + 1, count, perm_len, max_number);
	}
}

// ===== Caculate Best Path(Permutation List, missing values) ================================================
// [Params]:
//			x: 2-dimension array
//1213232312321122
//3232111213213121
//2111323231231213
//2323112323121213
//1213233321231231
//2111323132231222
//1123133212311123
//3233123132313211
//3112312312323121
//1232312312321113
//2132311232312123
//3121213232112312
//			b: scan range (bottom) 
//			t: scan range (top) 
//			p: permutation list
//			p_count: permutation count
int calcBestPath(BYTE **x, int b, int t, BYTE *p, int p_count, int *max_perm_num_missed, int digit_count, int max_row_count)
{
	int i = 0, j = 0, k = 0;
	int val = 0;
	BYTE *pp = NULL;
	int hist[4];
	int missed_value = -1;

	int max_len = 0;
	int len = 0;

	int max_perm_count = 0;

	for(k = 0; k < p_count; k++)
	{
		pp = p + k * digit_count;		
		missed_value = -1;

		//if( pp[0] == 13 && pp[1] == 13 && pp[2] == 13 )
		//	pp[0] = 13;
		for(i = b - 1, j = 0; i >= t - 1; i--, j++ )
		{	
			if( j % digit_count == 0 )
				memset(hist, 0, 4 * sizeof(int));

			val = x[i][pp[j % digit_count] - 1];
			hist[val]++;

			// check missed value
			if( j == digit_count - 1 )	// 3
			{
				if(hist[1] == 0 && hist[2] >= 1 && hist[3] >= 1 )
					missed_value = 1;
				else if(hist[1] >= 1 && hist[2] == 0 && hist[3] >= 1 )
					missed_value = 2;
				else if(hist[1] >= 1 && hist[2] >= 1 && hist[3] == 0 )
					missed_value = 3;	 

				if( missed_value < 0 ) // Breakdown
					break;		
			}
			else if( j < digit_count - 1 )
			{

			}
			else	// 
			{
				if( hist[missed_value] > 0 ) // Breakdown														
					break;

				if( j % 3 == digit_count - 1 )
				{
					int breakdown_flag = 0;
					for(int q = 1; q < 3; q++)
					{
						if( q == missed_value )
							continue;

						if( hist[q] == 0 )
						{
							breakdown_flag = 1;
							break;
						}
					}

					if( breakdown_flag == 1 )
						break;
				}
			}

			len = j + 1;
		}

		if( len > max_len )
		{
			memset(max_perm_num_missed, 0, 2 * max_row_count * sizeof(int));
			max_perm_count = 0;

			max_perm_num_missed[0] = k;
			max_perm_num_missed[1] = missed_value;
			max_perm_count++;

			max_len = len;
		}
		else if( len >= max_len && len > 0 )
		{
			if( max_perm_count < max_row_count )
			{
				max_perm_num_missed[max_perm_count * 2] = k;
				max_perm_num_missed[max_perm_count * 2 + 1] = missed_value;
				max_perm_count++;
			}
		}
	}

	return max_len;
}

CString calcPath(BYTE **x, int row, int col, int upto)
{
	CString ret;

	int i = 0, j = 0, k = 0;

	long int  start = GetTickCount();

	int *max_perm_num_missed = (int *) calloc(MAX_PERM_COUNT * 2, sizeof(int));

	for(k = 3; k <= upto; k++)
	{
		int PERM_TOTAL_COUNT  = 1;

		for(i = 0; i < k; i++)
			PERM_TOTAL_COUNT *= col;

		size_t size_len = PERM_TOTAL_COUNT * k;

		BYTE *perm_list = (BYTE*)calloc(size_len, sizeof(BYTE));
		if( perm_list == NULL )
		{
			CString msg;
			msg.Format("\r\n\r\nPlets %d Memory Error", k);
			ret += msg;

			return ret;
		}

		// generate possible permutation list
		int count = 0;
		generatePermList(perm_list, 0, count, k, col);

		// initialize result buffer
		memset(max_perm_num_missed, 0, MAX_PERM_COUNT * 2 * sizeof(int));

		int max_len = calcBestPath(x, row, 1, perm_list, PERM_TOTAL_COUNT, max_perm_num_missed, k, MAX_PERM_COUNT);

		CString msg;
		msg.Format("\r\n-Digits: %d\r\n", k);
		ret += msg;

		CString sub_ret;
		int max_perm_count = 0;
		for(i = 0; i < MAX_PERM_COUNT; i++)
		{
			int p_num = max_perm_num_missed[i * 2];
			if(max_perm_num_missed[i * 2 + 1] < 1 )
				break;

			BYTE *p = perm_list + p_num * k;

			sub_ret += "\r\n(";
			for(j = 0; j < k; j++)
			{
				if( j > 0 )
					sub_ret += ",";		

				msg.Format("%d", p[j]);
				sub_ret += msg;			
			}

			sub_ret += ")";

			float fHeight = (float)max_len / k;
			msg.Format("\r\nHeight: %f\r\nMissing: %d\r\n",
						fHeight, max_perm_num_missed[i * 2 + 1]
					);

			sub_ret += msg;

			max_perm_count++;
		}

		msg.Format("\r\nTotal Count: %d", max_perm_count);		
		ret += msg;
		ret += "\r\n\r\n";
		ret += sub_ret;

		ret += "\r\n";
		
		free(perm_list);
	}

	free(max_perm_num_missed);

	long int  end = GetTickCount();

	ret += "---Total Time: ";
	float gap = (end - start) / 1000;
	CString msg;
	msg.Format("%1.2fs", gap);
	ret += msg;	

	return ret;
}