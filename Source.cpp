#include <iostream>
using namespace std;

int f(int C)
{
	int temp = (C - 1)&C;
	int temp2 = !temp;
	return temp2;
}

int main()
{
	////int i;
	////cout << "Please enter an integer value: ";
	////cin >> i;
	////cout << "The value you entered is " << i;
	////cout << " and its double is " << i * 2 << ".\n";
	////return 0;

	int result = f(-32768);
}