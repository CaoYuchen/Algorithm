#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
using namespace std;

int n = 0, m = 0, k = 0, l = 0;
int ****d;
int n_max, m_max, k_max, l_max;
int planemax(int & x, int & y, int & z, int w);
void peakfind(int left, int right);

int main()
{
	
	ifstream in("sample.txt", ios::binary);
	

	in >> m_max;
	in >> l_max;
	in >> k_max;
	in >> n_max;



	d = new int***[n_max + 2];
	for (int i = 0; i < n_max + 2; i++)
		d[i] = new int**[m_max + 2];
	for (int i = 0; i < n_max + 2; i++)
	for (int j = 0; j < m_max + 2; j++)
		d[i][j] = new int*[k_max + 2];
	for (int i = 0; i < n_max + 2; i++)
	for (int j = 0; j < m_max + 2; j++)
	for (int r = 0; r < k_max + 2; r++)
		d[i][j][r] = new int[l_max + 2];


	for (int s = 0; s < l_max+2; s++)
	for (int r = 0; r < k_max+2; r++)
	for (int j = 0; j < m_max+2; j++)
	for (int i = 0; i < n_max+2; i++)
		d[i][j][r][s] = 0;


	for (int s = 1; s < l_max+1; s++)
	for (int r = 1; r < k_max+1; r++)
	for (int j = 1; j < m_max+1; j++)
	for (int i = 1; i < n_max+1; i++)
		in >> d[i][j][r][s];

	in.close();

	cout << "Sample input£º" << endl;
	cout << m_max << ' ';
	cout << l_max << ' ';
	cout << k_max << ' ';
	cout << n_max << ' ';

	for (int s = 1; s < l_max+1; s++)
	for (int r = 1; r < k_max+1; r++)
	for (int j = 1; j < m_max+1; j++)
	{
		cout << endl;
		for (int i = 1; i < n_max+1; i++)
			cout << d[i][j][r][s] << ' ';
	}


	cout << endl << "Sample output£º" << endl;
	peakfind(0, l_max+1);


	//	int * a =(int *)malloc(sizeof(int)*(int)dimens[0]);
	//	cout << (int)dimens[0]<<endl<<a << endl;

	cout << "Running time of Algorithm£ºT(n)=T(n/2)+n+2,so T(n)=O(nlogn)" << endl;


	for (int r = 1; r < k_max + 2; r++)
	for (int j = 1; j < m_max + 2; j++)
	for (int i = 1; i < n_max + 2; i++)
	{
		delete[] d[i][j][r];
		d[i][j][r] = NULL;
	}

	for (int j = 1; j < m_max + 2; j++)
	for (int i = 1; i < n_max + 2; i++)
	{
		delete[] d[i][j];
		d[i][j] = NULL;
	}

	for (int i = 1; i < n_max + 2; i++)
	{
		delete[] d[i];
		d[i] = NULL;
	}
		delete[] d;
		d = NULL;

	return 0;
}

void peakfind(int left, int right)
{
	if (left == right)
	{
		cout << "Cannot find any peak" << endl;
		return;
	}
	l = (left + right) / 2;

	int k_0 = planemax(n, m, k, l - 1);
	int k_2 = planemax(n, m, k, l + 1);
	int k_1 = planemax(n, m, k, l);

	if (
		(n > 0 && m > 0 && k > 0 &&
		d[n][m][k][l] >
		d[n][m][k][l - 1]) &&

		(n < n_max+2 && m < m_max+2 && k < k_max+2 &&
		d[n][m][k][l] >
		d[n][m][k][l + 1]) &&

		(l > 0 && l < l_max+2)
		)
	{
		cout << m << ' ' << l << ' ' << k << ' ' << n << endl;
		return;
	}
	else if (l > 0 && (k_0 > k_1))
	{
		right = l;
		peakfind(left, right);
		return;
	}
	else if (l < l_max+2 && (k_2>k_1))
	{
		left = l+1;
		peakfind(left, right);
		return;
	}


}

int planemax(int & x, int & y, int & z, int w)
{
	int max = 0;
	for (int f = 1; f < k_max+1; f++)
	for (int j = 1; j < m_max+1; j++)
	for (int i = 1; i < n_max+1; i++)
	if (max < d[i][j][f][w])
	{
		max = d[i][j][f][w];
		x = i, y = j, z = f;
	}
	return max;
}



