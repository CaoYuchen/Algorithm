#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<fstream>

using namespace std;

void LCS(int* X, int* Y, int m, int n);
void Print(int** b, int* X, int i, int j);
int Out[450];

int main()
{
	//string xx = "ABCDEFGHIJKLMN";
	//string yy = "DEFGHIJKOPQWER";
	//int a[10];
	//cout << sizeof(a) / sizeof(a[0]) << endl;
	//cout << xx.length() << endl;

	int xx[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int yy[10] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

	ifstream in_L("L.txt", ios::binary);
	ifstream in_R("R.txt", ios::binary);
	ofstream out("out.txt");

	int X[450], Y[450];
	for (int i = 0; i < 375; i++)
	{
		for (int j = 0; j < 450; j++)
		{
			in_L >> X[j];
			in_R >> Y[j];
			Out[j] = 0;
		}
		
		LCS(X, Y, 450, 450);

		for (int k = 0; k < 450; k++)
		{
			out << Out[k]*255/50 << ' ';
			if (k == 449)
				out << endl;
		}
	}

//	LCS(xx, yy, 10, 10);
//	cout << endl;
	return 0;
}

void LCS(int* X, int* Y, int m, int n)
{
	//int m = X.length(), n = Y.length();
	
	int **b = new int*[m];
	for (int i = 0; i < m; i++)
		b[i] = new int[n];
	int **c = new int*[m+1];
	for (int i = 0; i < m+1; i++)
		c[i] = new int[n+1];

	for (int i = 0; i < m+1; i++)
		c[i][0] = 0;
	for (int i = 0; i < n+1; i++)
		c[0][i] = 0;

	for (int i = 1; i < m + 1; i++)
	{
		for (int j = 1; j < n + 1; j++)
		{
			//if (i < 3 || j < 3)
			if (X[i - 1]< Y[j - 1] + 10 && X[i - 1]>Y[j - 1] - 10)
			{
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i - 1][j - 1] = 1;
			}
			else if (c[i - 1][j] >= c[i][j - 1])
			{
				c[i][j] = c[i - 1][j];
				b[i - 1][j - 1] = 2;
			}
			else
			{
				c[i][j] = c[i][j - 1];
				b[i - 1][j - 1] = 3;
			}
		}
	}


	
	Print(b, X, m-1, n-1);



	for (int i = 0; i < m+1; i++)
	{
		delete[] c[i];
		c[i] = NULL;
	}
	delete[] c;
	c = NULL;

	for (int i = 0; i < m; i++)
	{
		delete[] b[i];
		b[i] = NULL;
	}
	delete[] b;
	b = NULL;

}

void Print(int**b, int* X, int i, int j)
{
	if (i == 0 || j == 0)
	{
		//cout << X[i];
		Out[i] = abs(i-j);
		return;
	}
	if (b[i][j] == 1)
	{
		Print(b, X, i - 1, j - 1);
		//cout << X[i];
		Out[i] = abs(i-j);
	}
	else if (b[i][j] == 2)
	{
		Print(b, X, i - 1, j);
		//cout << 0;
	}
	else
	{
		Print(b, X, i, j - 1);
		
	}
}