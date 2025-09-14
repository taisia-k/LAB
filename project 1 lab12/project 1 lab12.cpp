#include <iostream>
using namespace std;

int main()
{
	setlocale(LC_ALL, "rus");
	int arr[10];

	for (int i = 0; i < 10; i++)
	{
		cin >> arr[i];
	}
	cout << "Элементы массива: ";
	for (int i = 0; i < 10; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
	int sm = 0;
	cout << "Сумма элементов массива: ";
	for (int i = 0; i < 10; i++)
	{
		sm += arr[i];
	}
	cout << sm << endl;
	cout << "Максимальный элемент массива: ";
	int mx = arr[0];
	for (int i = 1; i < 10; i++)
	{
		if (arr[i] > mx) {
			mx = arr[i];
		}
	}
	cout << mx << endl;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9 - i; j++)
		{
			if (arr[j] < arr[j + 1]) {
				int t = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = t;
			}
		}
	}
	cout << "Отсортированные элементы массива ";
	for (int i = 0; i < 10; i++)
	{
		cout << arr[i] << " ";
	}
	return 0;
}