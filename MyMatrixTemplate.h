/*
	Name:              Austin Cathey
	Class:             CSCI 30, Advanced Programming in C++
	Section:           0124
	Assignment Number: 1
	File(s):           MyMatrix.h
	Due Date: 03/10/2009
	----------------------------------------------------------------------
	I, Austin Cathey, pledge that this is my own independent work,
	which conforms to generally accepted guidelines of academic honesty.
	----------------------------------------------------------------------
*/

// Make sure this file is only included once
#pragma once

// Includes
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <cstdlib>

// Use standard namespace
using namespace std;

// Necessary typedefs
typedef double MatrixCellType;
typedef vector<MatrixCellType> MatrixRowType;
typedef vector<MatrixRowType> MatrixData;

// The MyMatrix class
template <typename T>
class MyMatrix
{
	// Necessary typedefs
public:
	typedef typename T MatrixCellType;				// typename is required here
	typedef vector<MatrixCellType> MatrixRowType;
	typedef vector<MatrixRowType> MatrixData;

private:

	MatrixData m_data;
	MatrixCellType m_initial_value;
	int m_rows;
	int m_cols;
	int m_error_code;

	// Number of decimal places
	static int C_Precision;
	static int C_Width;
	static int C_Object_ID;

	// Initialization
	void ErrorCode(int error_code) { m_error_code = error_code; }	// Set error code.

	// Helper functions
	bool _Construct(int rows, int cols, MatrixCellType initial_value, int error_code);

	// Checks to see if the size is valid.
	static bool C_ValidSize(int rows, int cols)
	{
		return !rows || !cols || ((rows >= 2) && (cols >= 2));
	}

public:

	// Constants
	static const int ERR_NONE;
	static const int ERR_BAD_ALLOC;
	static const int ERR_BAD_ARGS;
	static const int ERR_DIFFERENT_SIZES;
	static const int ERR_NOT_SQUARE_MATRIX;
	static const int ERR_FILE_ERROR;
	static const int ERR_EARLY_EOF;

	// Constructors and destructor
	explicit MyMatrix(int rows = 0, int cols = 0, MatrixCellType initial_value = MatrixCellType());
	explicit MyMatrix(const char *const filename);
	explicit MyMatrix(istream &is);

	// Error stuff
	int ErrorCode(void) const { return m_error_code; }
	void ClearError(void) { ErrorCode(ERR_NONE); }
	const char *ErrorString(void) const;
	void WriteErrorString(ostream &os) const;

	// Precision
	static void Precision(int p) { MyMatrix::C_Precision = p; }
	static int Precision(void) { return MyMatrix::C_Precision; }
	static void Width(int w) { MyMatrix::C_Width = w; }
	static int Width(void) { return MyMatrix::C_Width; }

	// Overloaded operators
	const MyMatrix &operator +=(const MyMatrix &mat);
	const MyMatrix &operator -=(const MyMatrix &mat);
	MyMatrix operator +(const MyMatrix &mat) const;
	MyMatrix operator -(const MyMatrix &mat) const;

	// Test conditions
	bool IsEmpty(void) const { return m_data.empty() || m_rows <= 0 || m_cols <= 0; }
	bool IsValid(void) const { return MyMatrix::C_ValidSize(m_rows, m_cols); }
	bool IsNoError(void) const { return ErrorCode() == ERR_NONE; }
	bool IsGood(void) const { return (IsValid() || IsEmpty()) && IsNoError(); }
	bool operator !(void) const { return !IsGood(); }
	bool IsSameSize(const MyMatrix &mat) const { return (Rows() == mat.Rows()) && (Columns() == mat.Columns()); }
	bool Equals(const MyMatrix &mat) const;

	// Operators
	MatrixRowType &operator [](int index) { return m_data[index]; }
	const MatrixRowType &operator [](int index) const { return m_data[index]; }

	// Matrix size
	void GetSize(int &rows, int &cols) const { rows = Rows(); cols = Columns(); }
	bool SetSize(int rows, int cols);

	// Accessors/Mutators
	int Rows(void) const { return m_rows >= 0 ? m_rows : 0; }
	int Columns(void) const { return m_cols >= 0 ? m_cols : 0; }
	bool Rows(int rows) { return SetSize(rows, abs(m_rows)); }
	bool Columns(int cols) { return SetSize(abs(m_cols), cols); }
	bool DeleteRow(int row);
	bool DeleteColumn(int column);

	// Other member functions
	MyMatrix Submatrix(int row_delete, int col_delete) const;
	MyMatrix &Submatrix(int row_delete, int col_delete, MyMatrix &sub) const;
	MatrixCellType Determinant(void) const;

	// Generates a random matrix
	void GenerateRandom(int rows, int cols, int n);

	// Convert matrix row to string.
	stringstream &RowSStream(int row, bool to_screen, stringstream &ss) const;

	// Input and output
	istream &Read(istream &is);
	ostream &Write(ostream &os) const;
};

// Overloaded insertion/extraction operators.
template <typename T>
inline istream &operator >>(istream &is, MyMatrix<T> &m)
{
	return m.Read(is);
}
template <typename T>
inline ostream &operator <<(ostream &os, const MyMatrix<T> &m)
{
	return m.Write(os);
}

/////////////////////////////////
//                             //
// Constructors and Destructor //
//                             //
/////////////////////////////////

// MyMatrix::MyMatrix /////////////////////////////////////
template <typename T>
MyMatrix<T>::MyMatrix(int rows, int cols, typename MyMatrix<T>::MatrixCellType initial_value) :
m_rows(0), m_cols(0)
{
	_Construct(rows, cols, initial_value, MyMatrix<T>::ERR_NONE);
}

// MyMatrix::MyMatrix /////////////////////////////////////
template <typename T>
MyMatrix<T>::MyMatrix(const char *const filename)
{
	ifstream infile;

	// Iniitalize our brand spanking new object
	_Construct(0, 0, MatrixCellType(), MyMatrix<T>::ERR_NONE);

	// Read from file
	infile.open(filename);
	if (!infile)
	{
		ErrorCode(MyMatrix<T>::ERR_FILE_ERROR);
	}
	else
	{
		Read(infile);
		infile.close();
	}
}

// MyMatrix::MyMatrix /////////////////////////////////////
template <typename T>
MyMatrix<T>::MyMatrix(istream &is)
{
	_Construct(0, 0, MatrixCellType(), MyMatrix<T>::ERR_NONE);
	Read(is);
}

//////////////////////////
//                      //
// Overloaded operators //
//                      //
//////////////////////////

// MyMatrix::operator -= //////////////////////////////////
template <typename T>
const MyMatrix<T> &MyMatrix<T>::operator -=(const MyMatrix<T> &mat)
{
	// Only subtract if the matrices are of the same size.
	if (IsSameSize(mat))
	{
		for (int row = 0, nrows = Rows(); row < nrows; row++)
		{
			for (int col = 0, ncols = Columns(); col < ncols; col++)
			{
				(*this)[row][col] -= mat[row][col];
			}
		}
	}
	else
	{
		ErrorCode(MyMatrix<T>::ERR_DIFFERENT_SIZES);
	}
	return *this;
}

// MyMatrix::operator += //////////////////////////////////
template <typename T>
const MyMatrix<T> &MyMatrix<T>::operator +=(const MyMatrix<T> &mat)
{
	// Only add if the matrices are the same size.
	if (IsSameSize(mat))
	{
		for (int row = 0, nrows = Rows(); row < nrows; row++)
		{
			for (int col = 0, ncols = Columns(); col < ncols; col++)
			{
				(*this)[row][col] += mat[row][col];
			}
		}
	}
	else
	{
		ErrorCode(MyMatrix<T>::ERR_DIFFERENT_SIZES);
	}
	return *this;
}

// MyMatrix::operator + ///////////////////////////////////
template <typename T>
MyMatrix<T> MyMatrix<T>::operator +(const MyMatrix<T> &mat) const
{
	return MyMatrix<T>(*this) += mat;
}

// MyMatrix::operator - ///////////////////////////////////
template <typename T>
MyMatrix<T> MyMatrix<T>::operator -(const MyMatrix<T> &mat) const
{
	return MyMatrix<T>(*this) -= mat;
}

/////////////////////////////
//                         //
// Public Member Functions //
//                         //
/////////////////////////////

// MyMatrix::SetSize //////////////////////////////////////
template <typename T>
bool MyMatrix<T>::SetSize(int rows, int cols)
{
	MatrixRowType temp;

	// If either size is zero, then free the matrix.
	// The test is purposely set up like this so we don't
	// "store" two negative values.
	if (!rows || !cols)
	{
		m_data.clear();

		// We still need to store the information about the size
		// in the event we want to make a nonempty matrix.
		m_rows = -abs(rows);
		m_cols = -abs(cols);
	}

	// Otherwise, resize ONLY if a different size is passed.
	else if (rows >= 2 && cols >= 2)
	{
		// It was clearer to write it this way.
		if (rows != Rows() || cols != Columns())
		{
			// Create a temporary matrix and use that as the initial value
			// for any new rows that may be created.
			temp.assign(cols, m_initial_value);
			m_data.resize(rows, temp);

			// Create each row.
			for (int index = 0; index < rows; index++)
			{
				m_data[index].resize(cols, m_initial_value);
			}

			// Store the rows and columns.
			m_rows = rows;
			m_cols = cols;
		}
	}

	// The size was not valid, so set an error flag.
	else
	{
		ErrorCode(MyMatrix<T>::ERR_BAD_ARGS);
		return false;
	}

	// Success!
	return true;
}

// MyMatrix::DeleteRow ////////////////////////////////////
template <typename T>
bool MyMatrix<T>::DeleteRow(int row)
{ // Deletes a row from the matrix.

	// Delete the row if we have a valid row.
	if ((row >= 0) && (row < Rows()))
	{
		m_data.erase(m_data.begin() + row);

		// Update the counter and return success.
		m_rows--;
		return true;
	}
	return false;
}

// MyMatrix::DeleteColumn /////////////////////////////////
template <typename T>
bool MyMatrix<T>::DeleteColumn(int column)
{ // Deletes a column from the matrix.
	MatrixData::iterator current;
	MatrixData::iterator endpoint;

	// If we have a valid column . . .
	if ((column >= 0) && (column < Columns()))
	{
		// Delete the column from each row.
		for (current = m_data.begin(), endpoint = m_data.end(); current != endpoint; current++)
		{ 
			current->erase(current->begin() + column);
		}

		// Update the counter and return success.
		m_cols--;
		return true;
	}
	return false;
}

// MyMatrix::Submatrix ////////////////////////////////////
template <typename T>
MyMatrix<T> &MyMatrix<T>::Submatrix(int row_delete, int col_delete, MyMatrix<T> &sub) const
{ // Generates the submatrix of a matrix.  This form of the function was written to avoid calling the copy constructor.

	// Delete a row and a column
	if (!IsEmpty())
	{
		sub = *this;					// I opted for the easy way out.
		sub.DeleteRow(row_delete);
		sub.DeleteColumn(col_delete);
	}

	// Return the submatrix
	return sub;
}

// MyMatrix::Submatrix ////////////////////////////////////
template <typename T>
MyMatrix<T> MyMatrix<T>::Submatrix(int row_delete, int col_delete) const
{
	MyMatrix<T> m;
	return Submatrix(row_delete, col_delete, m);
}

// MyMatrix::Determinant //////////////////////////////////
template <typename T>
typename MyMatrix<T>::MatrixCellType MyMatrix<T>::Determinant(void) const
{ // Calculates the determinant of a submatrix.
	MatrixCellType sum = 0.0, temp;
	bool positive = true;
	int rows = Rows(), cols = Columns();

	// Square matrix
	if (rows == cols)
	{
		// Base case: Matrix is a 2x2 matrix.
		if (rows == 2)
		{
			return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
		}

		// New submatrix to avoid multiple calls to copy constructor
		MyMatrix<T> sub(rows - 1, cols - 1);

		// Expand about the first row.
		for (int column = 0; column < cols; column++)
		{
			// Only multiplying where necessary for efficiency.
			temp = (*this)[0][column] * Submatrix(0, column, sub).Determinant();

			// Update sum
			if (positive) {	sum += temp; }
			else { sum -= temp;	}
			positive = !positive;
		}
	}

	// Set error code
	else
	{
		// This should throw an exception or return a NaN value.
		T zero = T();
		sum /= zero;
	}

	// sum now holds the value of the determinant.
	return sum;
}

// MyMatrix::GenerateRandom ///////////////////////////////
template <typename T>
void MyMatrix<T>::GenerateRandom(int rows, int cols, int n)
{ // Creates a randomly generated matrix.
	if (SetSize(rows, cols))
	{
		for (int row = 0; row < rows; row++)
		{
			for (int col = 0; col < cols; col++)
			{
				(*this)[row][col] = rand() % n;
			}
		}
	}
}

// Constants
template <typename T> const int MyMatrix<T>::ERR_NONE = 0;
template <typename T> const int MyMatrix<T>::ERR_BAD_ALLOC = 1;
template <typename T> const int MyMatrix<T>::ERR_BAD_ARGS = 2;
template <typename T> const int MyMatrix<T>::ERR_DIFFERENT_SIZES = 3;
template <typename T> const int MyMatrix<T>::ERR_NOT_SQUARE_MATRIX = 4;
template <typename T> const int MyMatrix<T>::ERR_FILE_ERROR = 5;
template <typename T> const int MyMatrix<T>::ERR_EARLY_EOF = 6;

//////////////////
//              //
// Error String //
//              //
//////////////////

// MyMatrix::ErrorString //////////////////////////////////
template <typename T>
const char *MyMatrix<T>::ErrorString(void) const
{
	static char *error_string[] = {
		"Nothing is wrong with this matrix.",
		"Allocation error.  The system is possibly out of memory.",
		"Operation failed due to bad arguments being passed.",
		"The matrices were of different sizes, so the operation failed.",
		"This operation only works if the matrix is a square matrix.",
		"The operation failed due to a file I/O error.",
		"The operation failed due to an unexpected EOF."
	};
	return error_string[ErrorCode()];
}

// MyMatrix::WriteErrorString /////////////////////////////
template <typename T>
void MyMatrix<T>::WriteErrorString(ostream &os) const
{ // Writes the error string only if an error is prevalent.
	if (ErrorCode())
	{
		os << ErrorString() << endl;
	}
}

///////////////////////
//                   //
// I/O-Related Stuff //
//                   //
///////////////////////

// MyMatrix::RowSStream ///////////////////////////////////
template <typename T>
stringstream &MyMatrix<T>::RowSStream(int row, bool to_screen, stringstream &ss) const
{ // Writes matrix information to a stringstream for easier processing.
	int cols = Columns();

	// Make screen output look pretty.
	if (to_screen) { ss << '[' << ' '; }

	// If the row has actual data inside of it . . .
	if (cols > 0)
	{
		// First element
		if (MyMatrix<T>::Width() > 0) { ss << setw(Width()); }
		ss << (*this)[row][0];

		// Display the rest of the row.
		for (int col = 1; col < cols; col++)
		{
			ss << ' ';
			if (MyMatrix<T>::Width() > 0) { ss << setw(Width()); }
			ss << (*this)[row][col];
		}

		// Display the space here for neatness
		if (to_screen) { ss << ' '; }
	}

	// Closing brace for screen output to look nice.
	if (to_screen) { ss << ']'; }

	// Return stream
	return ss;
}

// MyMatrix::Read /////////////////////////////////////////
template <typename T>
istream &MyMatrix<T>::Read(istream &is)
{
	int rows, cols;					// Size of matrix
	string s;						// For getline()
	bool keyboard = (&is == &cin);	// Am I reading from the keyboard?

	// Get rows
	if (keyboard) { cout << "Enter number of rows: "; }
	is >> rows;
	if (is.bad())
	{
		ErrorCode(MyMatrix<T>::ERR_FILE_ERROR);
		return is;
	}

	// Get columns
	if (keyboard) { getline(is, s); cout << "Enter number of columns: "; }
	is >> cols;
	if (is.bad())
	{
		ErrorCode(MyMatrix<T>::ERR_FILE_ERROR);
		return is;
	}

	// Resize the matrix
	if (!SetSize(rows, cols))
	{
		return is;
	}

	// Prompt user for matrix data
	if (keyboard)
	{
		getline(is, s);
		cout << "Enter the values for your " << rows << "x" << cols << " matrix." << endl;
	}

	// Read in matrix data.
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			is >> (*this)[row][col];
			if (!is)
			{
				ErrorCode(is.eof() ? MyMatrix<T>::ERR_EARLY_EOF : MyMatrix<T>::ERR_FILE_ERROR);
				return is;
			}
		}
	}

	// Get rid of any newline characters.
	if (keyboard) { getline(is, s); }

	// Return value for extraction operator.
	return is;
}

// MyMatrix::Write ////////////////////////////////////////
template <typename T>
ostream &MyMatrix<T>::Write(ostream &os) const
{
	bool screen;		// Am I writing to the screen?
	int rows;			// Number of rows in matrix
	stringstream ss;	// To make things easier on myself.

	// Spare me the multiple calls.
	rows = Rows();

	// Are we writing to the screen?
	screen = (&os == &cout) || (&os == &cerr);

	// Write the size of the matrix to a file if needed.
	if (!screen)
	{
		os << rows << ' ' << Columns() << endl;
	}

	// Only act on a non-empty matrix.
	if (!IsEmpty())
	{
		// Prepare the string stream.
		ss << setprecision(MyMatrix<T>::Precision()) << fixed << right << setfill(' ');
		RowSStream(0, screen, ss);
		for (int row = 1; row < rows; row++)
		{
			ss << '\n';
			RowSStream(row, screen, ss);
		}

		// Write to stream
		os << ss.str();
	}
	else if (screen)
	{
		os << "<Empty Matrix>";
	}

	// Return value of insertion operator
	return os << flush;
}

// Class variables
template <typename T> int MyMatrix<T>::C_Precision = 6;
template <typename T> int MyMatrix<T>::C_Width = -1;
template <typename T> int MyMatrix<T>::C_Object_ID = 0;

//////////////////////////////
//                          //
// Private Helper Functions //
//                          //
//////////////////////////////

// MyMatrix::_Construct ///////////////////////////////////
template <typename T>
bool MyMatrix<T>::_Construct(int rows, int cols, typename MyMatrix<T>::MatrixCellType initial_value, int error_code)
{ // Helper function for the constructor

	// Initialize fields directly
	m_initial_value = initial_value;
	m_error_code    = error_code;

	// Set the new size.
	return SetSize(rows, cols);
}