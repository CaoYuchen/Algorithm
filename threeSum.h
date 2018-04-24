// Given an integer k and an array of integers S, find out 3 numbers in the array, which sum of them is clostest to k.
// return the clostest sum.

// Inputs:
//  S: an integer array
//  len: length of the number array
//  k: the targe integer


#ifndef THREE_SUM_H_
#define THREE_SUM_H_

// void quicksort(int* &S, int left, int right);
void quicksort(int *a, int low, int high);
int threeSum(int* S, int len, int k)
{
    ////////////////////////////////////////////////////////
    // TODO: Finish your algorithm here             //
    // Hint: pre-sorting the number array may be helpful  //
    ////////////////////////////////////////////////////////

	
//	quicksort(S, 0, len-1);

	int min = 0, rec = 100;
	for (int l = 0; l < len; l++)
	{
		int kc = k - S[l];
		int i = 0, j = len - 1;

		while (i < j)
		{
			if (i == l)
				i++;
			if (j == l)
				j--;

			min = kc - S[i] - S[j];
			
			if (min > 0)
				i++;
			if (min < 0)
				j--;
			if (min == 0)
				return S[l] + S[i] + S[j];
			if (abs(min) < abs(rec))
				rec = min;

		}
	}
	return k - rec;
}




void quicksort(int* a, int low, int high)
{
	if (low >= high)
	{
		return;
	}
	int first = low;
	int last = high;
	int key = a[first];/*用字表的第一个记录作为枢轴*/

	while (first < last)
	{
		while (first < last && a[last] >= key)
		{
			--last;
		}

		a[first] = a[last];/*将比第一个小的移到低端*/

		while (first < last && a[first] <= key)
		{
			++first;
		}

		a[last] = a[first];
		/*将比第一个大的移到高端*/
	}
	a[first] = key;/*枢轴记录到位*/
	quicksort(a, low, first - 1);
	quicksort(a, first + 1, high);
}


/*
void quicksort(int* &S, int left, int right)
{
	int pivot = S[left], temp = 0, i = left, j = right, record = 0;
	
	while (i <= j)
	{
		if (i == j)	
			record = i;
		if (S[i] <= pivot)
			i++;
		if (S[j] >= pivot)
			j--;
		if (S[i] > pivot && S[j] < pivot)
		{
			temp = S[j];
			S[j] = S[i];
			S[i] = temp;
			i++, j--;
		}
	}
	
	if (record == left || record == right)
		return ;
	quicksort(S, record, right);
	quicksort(S, left, record);
}*/
#endif
