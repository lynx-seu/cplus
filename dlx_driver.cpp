#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dlx.h"
using namespace std;
#define block(r,c) (3*((r)/3)+((c)/3))
int cnt=0;
int grid[9][9];
void f(int x)
{
	int i;
	int c;
	int r;
	if (x+1) 
	{
		i=x%9; x/=9;
		c=x%9; r=x/9;
		grid[r][c]=i+1;
	}
	else
	{
		for (r=0; r<9; r++,putchar('\n'))
			for (c=0; c<9; c++)
				putchar(grid[r][c]+'0');
		printf("\n");
	}
}
int matrix[729][324];
void cover_col(int col)
{
	for (int row=0; row<729; row++)
		if (matrix[row][col])
		{
			matrix[row][col]=0;
			memset(matrix[row],0,sizeof(matrix[row]));
		}
}
void cover_row(int row)
{
	for (int col=0; col<324; col++)
		if (matrix[row][col])
		{
			matrix[row][col]=0;
			cover_col(col);
		}
}
int main()
{
	int row,r,c,i;
	char ch;
	for (row=0,r=0; r<9; r++)
		for (c=0; c<9; c++)
			for (i=0; i<9; i++,row++)
			{
				//uniqueness constraint
				matrix[row][r+9*c]=1;
				//row constraint
				matrix[row][81+i+9*r]=1;
				//column constraint
				matrix[row][162+i+9*c]=1;
				//block constraint
				matrix[row][243+i+9*block(r,c)]=1;
			}

	clock_t t1 = clock();
	const char * ttt = "800000000003600000070090200050007000000045700000100030001000068008500010090000400";		
	for (r=0; r<9; r++)
		for (c=0; c<9; c++)
		{
			int i = r*9+c;
			ch = ttt[i];
			if (ch>='1'&&ch<='9') //known value
			{
				cover_row(81*r+9*c+ch-'1');
				grid[r][c]=ch-'0';
			}
			else if (ch!='.'&&ch!='0') //placeholder
			{
				c--;
				continue;
			}
		}
	putchar('\n');
	DLX_dancing_links(729,324,(int*)&matrix,f);
	clock_t t2 = clock();
	printf("cost %dms\n", t2-t1);
	return 0;
}
