#include <iostream>
#include <iomanip>
#include <fstream>
#include "MyMatrixTemplate.h"

using namespace std;

int main(void)
{
	MyMatrix<float> m2(3, 3);
	ifstream fin("det.txt");

	fin >> m2;
	fin.close();

	cout << m2 << endl;

	cout << m2.Determinant() << endl;
	cout << m2.ErrorString() << endl;
}