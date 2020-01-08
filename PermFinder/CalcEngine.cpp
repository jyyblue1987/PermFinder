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

void generatePermList(BYTE *perm_list, int level, unsigned  long  long &count, int perm_len, int max_number)
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

void generatePermList1(unsigned  long  long &count, int perm_len, int max_number)
{	
	int pos = perm_len - 1;
	//BYTE *data = (BYTE *) calloc(perm_len, sizeof(BYTE));

	BYTE data[3];

	int i = 0;
	for(i = 0; i < perm_len; i++)
		data[i] = 1;

	data[perm_len - 1] = 0;
	
	while(true)
	{
		if( pos < 0 )
			break;

		if( data[pos] < max_number )
		{
			data[pos]++;
			if(pos < perm_len - 1)
				pos++;
			else
				count++;
		}
		else
		{
			data[pos] = 0;
			pos--;
		}
	}

	//free(data);
}

int calcBestPath1(BYTE **x, int b, int t, unsigned long long &p_count, BYTE *max_perm_num_missed, int digit_count, int max_number, int max_row_count)
{
	int i = 0, j = 0;
	unsigned long long k = 0;
	int val = 0;
	int hist[4];
	BYTE missed_value = 255;

	int max_len = 0;
	int len = 0;

	int max_perm_count = 0;

	int pos = digit_count - 1;
	BYTE *pp = (BYTE *) calloc(digit_count, sizeof(BYTE));

	for(i = 0; i < digit_count; i++)
		pp[i] = 1;

	pp[digit_count - 1] = 0;

	while(true)
	{
		if( pos < 0 )
			break;

		if( pp[pos] < max_number )
		{
			pp[pos]++;
			if(pos < digit_count - 1)
				pos++;
			else
			{
				if( pp[0] == 6 && pp[1] == 2 && pp[2] == 11 )
					pp[0] = 6;

				// main process
				missed_value = 255;
				memset(hist, 0, 4 * sizeof(int));
				for(i = b - 1, j = 0; i >= t - 1; i--, j++ )
				{	
					val = x[i][pp[j % digit_count] - 1];
					hist[val]++;

					BYTE bin_value = 255;
					for(int q = 1; q <= 3; q++)
					{
						if( hist[q] == 0 )
						{
							bin_value = q;
							break;
						}
					}

					if( bin_value == 255 )
						break;

					missed_value = bin_value;

					len = j + 1;
				}

				if( len > max_len )
				{
					memset(max_perm_num_missed, 0, (digit_count + 1) * max_row_count * sizeof(BYTE));
					max_perm_count = 0;

					memcpy(max_perm_num_missed, pp, digit_count * sizeof(BYTE));					
					max_perm_num_missed[digit_count] = missed_value;
					max_perm_count++;

					max_len = len;
				}
				else if( len >= max_len && len > 0 )
				{
					if( max_perm_count < max_row_count )
					{						
						memcpy(max_perm_num_missed + max_perm_count * (digit_count + 1), pp, digit_count * sizeof(BYTE));	
						max_perm_num_missed[max_perm_count * (digit_count + 1) + digit_count] = missed_value;
						max_perm_count++;
					}
				}

				p_count++;
			}
		}
		else
		{
			pp[pos] = 0;
			pos--;
		}
	}

	free(pp);

	return max_len;
}
int calcBestPathHistogram(BYTE **x, int b, int t, unsigned long long &p_count, int **end_missed_hist, int digit_count, int max_number)
{
	int i = 0, j = 0;
	unsigned long long k = 0;
	int val = 0;
	int hist[4];
	BYTE missed_value = 255;

	int max_len = 0;
	int len = 0;

	int pos = digit_count - 1;
	BYTE *pp = (BYTE *) calloc(digit_count, sizeof(BYTE));

	for(i = 0; i < digit_count; i++)
		pp[i] = 1;

	pp[digit_count - 1] = 0;

	while(true)
	{
		if( pos < 0 )
			break;

		if( pp[pos] < max_number )
		{
			pp[pos]++;
			if(pos < digit_count - 1)
				pos++;
			else
			{
				if( pp[0] == 6 && pp[1] == 2 && pp[2] == 11 )
					pp[0] = 6;

				// main process
				missed_value = 255;
				memset(hist, 0, 4 * sizeof(int));
				for(i = b - 1, j = 0; i >= t - 1; i--, j++ )
				{	
					val = x[i][pp[j % digit_count] - 1];
					hist[val]++;

					BYTE bin_value = 255;
					for(int q = 1; q <= 3; q++)
					{
						if( hist[q] == 0 )
						{
							bin_value = q;
							break;
						}
					}

					if( bin_value == 255 )
						break;

					missed_value = bin_value;

					len = j + 1;
				}

				if( len > max_len )
				{
					p_count = 0;

					for(i = 0; i < 3; i++)
						memset(end_missed_hist[i], 0, max_number * sizeof(int));

					end_missed_hist[missed_value - 1][pp[digit_count - 1] - 1]++;

					max_len = len;
					p_count++;
				}
				else if( len >= max_len && len > 0 )
				{
					end_missed_hist[missed_value - 1][pp[digit_count - 1] - 1]++;
					p_count++;
				}
			}
		}
		else
		{
			pp[pos] = 0;
			pos--;
		}
	}

	free(pp);

	return max_len;
}

int calcBestPathLenCount(BYTE **x, int b, int t, unsigned long long &p_count, int digit_count, int max_number)
{
	int i = 0, j = 0;
	unsigned long long k = 0;
	int val = 0;
	int hist[4];
	BYTE missed_value = 255;

	int max_len = 0;
	int len = 0;

	int pos = digit_count - 1;
	BYTE *pp = (BYTE *) calloc(digit_count, sizeof(BYTE));

	for(i = 0; i < digit_count; i++)
		pp[i] = 1;

	pp[digit_count - 1] = 0;

	while(true)
	{
		if( pos < 0 )
			break;

		if( pp[pos] < max_number )
		{
			pp[pos]++;
			if(pos < digit_count - 1)
				pos++;
			else
			{
				if( pp[0] == 6 && pp[1] == 2 && pp[2] == 11 )
					pp[0] = 6;

				// main process
				missed_value = 255;
				memset(hist, 0, 4 * sizeof(int));
				for(i = b - 1, j = 0; i >= t - 1; i--, j++ )
				{	
					val = x[i][pp[j % digit_count] - 1];
					hist[val]++;

					BYTE bin_value = 255;
					for(int q = 1; q <= 3; q++)
					{
						if( hist[q] == 0 )
						{
							bin_value = q;
							break;
						}
					}

					if( bin_value == 255 )
						break;

					missed_value = bin_value;

					len = j + 1;
				}

				if( len > max_len )
				{
					p_count = 0;

					max_len = len;
					p_count++;
				}
				else if( len >= max_len && len > 0 )
				{					
					p_count++;
				}
			}
		}
		else
		{
			pp[pos] = 0;
			pos--;
		}
	}

	free(pp);

	return max_len;
}

CString calcPath(BYTE **x, int row, int col, int upto)
{
	CString ret;

	int i = 0, j = 0, k = 0;

	DWORD  start = GetTickCount();

	BYTE *max_perm_num_missed = (BYTE *) calloc(MAX_PERM_COUNT * (upto + 1), sizeof(BYTE));

	for(k = upto; k >= 3; k--)
	{
		unsigned long long PERM_TOTAL_COUNT  = 1;

		for(i = 0; i < k; i++)
			PERM_TOTAL_COUNT *= col;

		// initialize result buffer
		memset(max_perm_num_missed, 0, MAX_PERM_COUNT * (k + 1) * sizeof(BYTE));

		// generate possible permutation list
		unsigned  long  long count = 0;
		int max_len = calcBestPath1(x, row, 1, count, max_perm_num_missed, k, col, MAX_PERM_COUNT);

		CString msg;
		msg.Format("------------------------------------------------------------\r\n-Digits: %d\r\n", k);
		ret += msg;

		CString sub_ret;
		int max_perm_count = 0;
		for(i = 0; i < MAX_PERM_COUNT; i++)
		{	
			BYTE *p = max_perm_num_missed + i * (k + 1);
			BYTE missed_value = p[k];
			if( missed_value < 1 )
				break;

			sub_ret += "\r\n(";
			for(j = 0; j < k; j++)
			{
				if( j > 0 )
					sub_ret += ",";		

				msg.Format("%d", p[j]);
				sub_ret += msg;			
			}

			sub_ret += ")";

			msg.Format("\r\nMissing: %d\r\n",
						missed_value
					);

			sub_ret += msg;

			max_perm_count++;
		}

		msg.Format("\r\nMax Length: %d", max_len);		
		ret += msg;

		float fHeight = (float)max_len / k;
		msg.Format("\r\nHeight: %1.2f", fHeight );
		ret += msg;

		msg.Format("\r\nTotal Count: %d", max_perm_count);		
		ret += msg;
		ret += "\r\n";
		ret += sub_ret;

		ret += "\r\n";
	}

	free(max_perm_num_missed);

	DWORD  end = GetTickCount();

	ret += "---Total Time: ";
	float gap = (float)(end - start) / 1000;
	CString msg;
	msg.Format("%1.2fs", gap);
	ret += msg;	

	return ret;
}

CString calcPathTesting(BYTE **x, int row, int col, int start_row, int upto)
{
	CString ret;
	CString msg;

	int i = 0, j = 0, k = 0;

	DWORD  start = GetTickCount();

	BYTE *max_perm_num_missed = (BYTE *) calloc(MAX_PERM_COUNT * (upto + 1), sizeof(BYTE));

	int *hist = (int *)calloc(row + 1, sizeof(int));

	for(k = 3; k <= upto; k++)
	{
		memset(hist, 0, (row + 1) * sizeof(int));

		ret += "-----------------------------------------------------------------------\r\n";
		msg.Format("%d-plets", k);
		ret += msg;

		for(int r = start_row; r < row; r++)
		{
			// initialize result buffer
			memset(max_perm_num_missed, 0, MAX_PERM_COUNT * (k + 1) * sizeof(BYTE));

			// generate possible permutation list
			unsigned  long  long count = 0;
			int max_len = calcBestPath1(x, r + 1, 1, count, max_perm_num_missed, k, col, MAX_PERM_COUNT);
			hist[max_len]++;
		}

		for(i = row; i >= 0; i-- )
		{
			if( hist[i] == 0 )
				continue;

			float fHeight = (float)i / k;
			msg.Format("\r\nHeight: %1.2f Length: %d Times: %d", fHeight, i, hist[i]);		
			ret += msg;
		}

		ret += "\r\n";
	}

	free(max_perm_num_missed);
	free(hist);

	DWORD  end = GetTickCount();

	ret += "\r\n---Total Time: ";
	float gap = (float)(end - start) / 1000;
	msg.Format("%1.2fs", gap);
	ret += msg;	

	return ret;
}

CString calcPathWithCompact(BYTE **x, int row, int col, int upto)
{
	CString ret;

	int i = 0, j = 0, k = 0;

	DWORD  start = GetTickCount();

	int *end_missed_hist[3];
	for(i = 0;  i < 3; i++)
		end_missed_hist[i] = (int *) calloc(col, sizeof(int));

	for(k = upto; k >= 3; k--)
	{
		unsigned long long PERM_TOTAL_COUNT  = 1;

		for(i = 0; i < k; i++)
			PERM_TOTAL_COUNT *= col;

		// initialize result buffer
		for(i = 0;  i < 3; i++)
			memset(end_missed_hist[i], 0, col * sizeof(int));

		// generate possible permutation list
		unsigned  long  long count = 0;
		int max_len = calcBestPathHistogram(x, row, 1, count, end_missed_hist, k, col);

		CString msg;
		msg.Format("------------------------------------------------------------\r\n-Digits: %d\r\n", k);
		ret += msg;

		CString sub_ret;
		int max_perm_count = count;

		for( i = 0; i < 3; i++)
		{
			for(j = 0; j < col; j++)
			{
				if(end_missed_hist[i][j] > 0)
				{
					msg.Format("\r\n(Number of permutations ending in %d: %d\r\nMissing number: %d",
							j + 1, end_missed_hist[i][j], i + 1);
					sub_ret += msg;
				}
			}
		}
		
		msg.Format("\r\nMax Length: %d", max_len);		
		ret += msg;

		float fHeight = (float)max_len / k;
		msg.Format("\r\nHeight: %1.2f", fHeight );
		ret += msg;

		msg.Format("\r\nTotal Count: %d", max_perm_count);		
		ret += msg;
		ret += "\r\n";
		ret += sub_ret;

		ret += "\r\n";
	}

	for(i = 0; i < 3; i++)
		free(end_missed_hist[i]);

	DWORD  end = GetTickCount();

	ret += "---Total Time: ";
	float gap = (float)(end - start) / 1000;
	CString msg;
	msg.Format("%1.2fs", gap);
	ret += msg;	

	return ret;
}

CString calcPathMaxLenTotal(BYTE **x, int row, int col, int start_row, int upto)
{
	CString ret;
	CString msg;

	int i = 0, j = 0, k = 0;

	DWORD  start = GetTickCount();

	for(k = upto; k <= upto; k++)
	{
		ret += "-----------------------------------------------------------------------\r\n";
		msg.Format("%d-plets", k);
		ret += msg;

		for(int r = start_row; r < row; r++)
		{
			// generate possible permutation list
			unsigned  long  long count = 0;
			int max_len = calcBestPathLenCount(x, r + 1, 1, count, k, col);		

			msg.Format("\r\n%d)  %d         %d", r - start_row + 1, max_len, count);
			ret += msg;
		}

		ret += "\r\n";
	}

	DWORD  end = GetTickCount();

	ret += "\r\n---Total Time: ";
	float gap = (float)(end - start) / 1000;
	msg.Format("%1.2fs", gap);
	ret += msg;	

	return ret;
}