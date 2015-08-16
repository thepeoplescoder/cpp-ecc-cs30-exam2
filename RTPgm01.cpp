//------------------------------------------------------------------------------
// RTPgm02.cpp
//
// MyMatrix(); ............................................................ DONE
// MyMatrix(int rows, int columns); ....................................... DONE
// MyMatrix(MyMatrix const &m); ........................................... DONE
// void Read(istream &is); ................................................ DONE
// void Write(ostream &os) const; ......................................... DONE
// double* operator[](int n); // Accessor & Mutator ....................... DONE
// double* operator[](int n) const; // Accessor Only ...................... DONE
// MyMatrix operator+(const MyMatrix &m) const; ........................... DONE
// MyMatrix operator-(const MyMatrix &m) const; ........................... DONE
// MyMatrix& operator=(const MyMatrix &m); ................................ DONE
// double Determinant() const; ............................................ DONE
// int Rows() const; ...................................................... DONE
// int Columns() const; ................................................... DONE
//
//------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include "MyMatrixTemplate.h"
//------------------------------------------------------------------------------
MyMatrix<double> ReadOne(istream& is);
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
int main()
{
	//-------------------------------------------------------------------------
	// Check out the constructors.
	MyMatrix<double> m1, m2(2,3), m3;
	//*
	cout << "After:\n"
		<< "       MyMatrix m1, m2(2,3), m3;\n"
		<< "m1:\n" << m1
		<< "\nm2:\n" << m2
		<< "\nm3:\n" << m3
		<< "\n\nHit enter to continue: " << flush;
	cin.get();
	cout << "\nNow to test the copy constructor.\n";
	m3 = ReadOne(cin); // Copy Constructor
	cout << "After:\n"
		<< "       m3 = ReadOne(cin);\n"
		<< "m3:\n" << m3
		<< "\nHit enter (once, maybe twice) to continue: " << flush;
	cin.get();
	cin.get();
	//-------------------------------------------------------------------------
	// Check out the Read and Write functions via the overloaded input and
	// output operators.
	//*/
	ifstream inputFile;
	//*
	inputFile.open("in.txt");
	inputFile >> m1 >> m2 >> m3;
	cout << "After:\n"
		<< "       ifstream inputFile;\n"
		<< "       inputFile.open(\"in.txt\");\n"
		<< "       inputFile >> m1 >> m2 >> m3;\n"
		<< "m1:\n" << m1
		<< "\nm2:\n" << m2
		<< "\nm3:\n" << m3
		<< "\nHit enter to continue: " << flush;
	cin.get();
	inputFile.close();
	inputFile.clear();
	//*
	ofstream outputFile;
	outputFile.open("out.txt");
	outputFile << m1 << '\n' << m2 << '\n' << m3;
	outputFile.close();
	outputFile.clear();

	MyMatrix<double> m4, m5, m6;
	inputFile.open("out.txt");
	inputFile >> m4 >> m5 >> m6;
	cout << "After:\n"
		<< "       inputFile.close();\n"
		<< "       inputFile.clear();\n"
		<< "       ofstream outputFile;\n"
		<< "       outputFile.open(\"out.txt\");\n"
		<< "       outputFile << m1 << ' ' << m2 << ' ' << m3;\n"
		<< "       outputFile.close();\n"
		<< "       outputFile.clear();\n"
		<< "       MyMatrix m4, m5, m6;\n"
		<< "       inputFile.open(\"out.txt\");\n"
		<< "       inputFile >> m4 >> m5 >> m6;\n"
		<< "m1:\n" << m1
		<< "\nm4:\n" << m4
		<< "\nm2:\n" << m2
		<< "\nm5:\n" << m5
		<< "\nm3:\n" << m3
		<< "\nm6:\n" << m6
		<< "\nHit enter (once, maybe twice) to continue: " << flush;
	cin.get();
	inputFile.close();
	inputFile.clear();
	//*-------------------------------------------------------------------------
	// Check out the overloaded subscript operator.
	cout << "\nSubscript operator."
		<< "\nInvoking:"
		<< "\n         cout << m2[0][0] << ' ' << m2[0][1] << ' ' << "
		<< "m2[0][2] << '\\n';"
		<< "\nresults in: " << flush;
	cout << m2[0][0] << ' ' << m2[0][1] << ' ' << m2[0][2] << '\n';
	cout << "\nInvoking:"
		<< "\n         cout << m2[1][0] << ' ' << m2[1][1] << ' ' << "
		<< "m2[1][2] << '\\n';"
		<< "\nresults in: " << flush;
	cout << m2[1][0] << ' ' << m2[1][1] << ' ' << m2[1][2] << '\n';
	cout << "\n\nHit enter to continue: " << flush;
	cin.get();
	m2[0][0] = 1.0;
	m2[0][1] = 2.0;
	m2[0][2] = 3.0;
	m2[1][0] = 4.0;
	m2[1][1] = 5.0;
	m2[1][2] = 6.0;
	cout << "After:\n"
		<< "       m2[0][0] = 1.0;\n"
		<< "       m2[0][1] = 2.0;\n"
		<< "       m2[0][2] = 3.0;\n"
		<< "       m2[1][0] = 4.0;\n"
		<< "       m2[1][1] = 5.0;\n"
		<< "       m2[1][2] = 6.0;\n"
		<< "m2:\n" << m2
		<< "\nHit enter to continue: " << flush;
	cin.get();
	const MyMatrix<double> cm2(m2); // Test subscript operator for a const object.
	cout << "\nAfter:"
		<< "\n      const cm2(m2);"
		<< "\nInvoking:"
		<< "\n         cout << cm2[0][0] << ' ' << cm2[0][1] << ' ' << "
		<< "cm2[0][2] << '\\n';"
		<< "\nresults in: " << flush;
	cout << cm2[0][0] << ' ' << cm2[0][1] << ' ' << cm2[0][2] << '\n';
	cout << "\nInvoking:"
		<< "\n         cout << cm2[1][0] << ' ' << cm2[1][1] << ' ' << "
		<< "cm2[1][2] << '\\n';"
		<< "\nresults in: " << flush;
	cout << cm2[1][0] << ' ' << cm2[1][1] << ' ' << cm2[1][2] << '\n';
	cout << "\n\nHit enter to continue: " << flush;
	cin.get();

	//-------------------------------------------------------------------------
	// Operator +
	m3 = m1 + m2;
	cout << "\nGiven m1:\n" << m1 << "\nand m2:\n" << m2
		<< "\ninvoking:"
		<< "\n         m3 = m1 + m2;\n"
		<< "results in m3:\n" << m3
		<< "\nHit enter to continue: " << flush;
	cin.get();
	const MyMatrix<double> cm1(m1);
	m3 = cm1 + cm2;
	cout << "\nTesting addition of const objects."
		<< "\nAfter:"
		<< "\n      const MyMatrix cm1(m1);"
		<< "\n      m3 = cm1 + cm2;"
		<< "\nGiven cm1:\n" << cm1 << "\nand cm2:\n" << cm2
		<< "\nm3 follows:\n" << m3
		<< "\nHit enter to continue: " << flush;
	cin.get();
	//-------------------------------------------------------------------------
	// Operator -
	m3 = m1 - m2;
	cout << "\nGiven m1:\n" << m1 << "\nand m2:\n" << m2
		<< "\ninvoking:"
		<< "\n         m3 = m1 - m2;\n"
		<< "results in m3:\n" << m3
		<< "\nHit enter to continue: " << flush;
	cin.get();
	m3 = cm1 - cm2;
	cout << "\nTesting subtraction of const objects."
		<< "\nAfter:"
		<< "\n      m3 = cm1 - cm2;"
		<< "\nGiven cm1:\n" << cm1 << "\nand cm2:\n" << cm2
		<< "\nm3 follows:\n" << m3
		<< "\nHit enter to continue: " << flush;
	cin.get();
	//-------------------------------------------------------------------------
	// Operator =
	m1 = m2;
	cout << "\nAfter:"
		<< "\n      m1 = m2;"
		<< "\nm1:\n" << m1
		<< "\nm2:\n" << m2
		<< "\nHit enter to continue: " << flush;
	cin.get();
	m1[0][0] = -999;
	cout << "\nAfter:\n"
		<< "       m1[0][0] = -999;\n"
		<< "m1:\n" << m1
		<< "\nm2:\n" << m2
		<< "\nHit enter to continue: " << flush;
	cin.get();
	//*/
	//-------------------------------------------------------------------------
	// Determinant & Rows & Columns
	inputFile.open("det.txt");
	inputFile >> m1;
	cout << "\nAfter:"
		<< "\n      inputFile.open(\"det.txt\");"
		<< "\n      inputFile >> m1;"
		<< "\nm1:\n" << m1
		<< "\nm1.Determinant() = " << m1.Determinant()
		<< "\n\nAlso, m1.Rows() = " << m1.Rows() << ", and m1.Columns() = "
		<< m1.Columns()
		<< "\nHit enter to continue: " << flush;
	cin.get();
	const MyMatrix<double> cm3(m1);
	cout << "\nAfter:"
		<< "\n      const cm3(m1);"
		<< "\ncm3:\n" << cm3
		<< "\ncm3.Determinant() = " << cm3.Determinant()
		<< "\n\nAlso, cm3.Rows() = " << cm3.Rows() << ", and cm3.Columns() = "
		<< cm3.Columns();

	cout << "\n\nHit enter to exit the program: " << flush;
	cin.get();
	return 0;
}
//------------------------------------------------------------------------------
MyMatrix<double> ReadOne(istream& is)
{
	MyMatrix<double> temp;
	is >> temp;
	return temp; //Will invoke the Copy Constructor automatically.
}
