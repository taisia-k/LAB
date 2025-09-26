
//ЗАДАНИЕ 1
#include <iostream>
#include <fstream>
using namespace std;
int main()
{
	setlocale(LC_ALL, "rus");
	float x = 0.0;
	cin >> x;
	if (x > 50)
	{
		cout << "Топлива достаточно";

	}
	else if (20 < x < 50) {
		cout << "Рекомендуется заправка";
	}
	else {
		cout << "Топливо на исходе, срочно заправьтесь!";
	}
	return 0;
}





/*ЗАДАНИЕ 2


#include <iostream>
using namespace std;
int main()
{
	setlocale(LC_ALL, "rus");
	float x = 0.1;
	while (x >= 0.0)
	{
		cin >> x;
	}
	cout << "Ввели отрицательное число";
	return 0;
} */