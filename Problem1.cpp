/*
	problem1.cpp

	Problem 1 for Exam 2

	Austin Cathey
	Computer Science 30
*/

// Disable warnings
#define _CRT_SECURE_NO_WARNINGS

// Includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cctype>
#include <cstdio>
#include <numeric>			// For inner_product()


// Use standard namespace
using namespace std;

// Prototypes
string FloatToString(float f);
void ReadInformation(istream &is, vector<string> &vs, vector<float> &vf);
istream &GetInputStream(void);
void CloseInputStream(istream &is);
string AddData(string s1, string s2);
string MultiplyData(string s, float f);

// Main
int main(void)
{
	istream &is = GetInputStream();
	vector<string> vs;
	vector<float> vf;

	// If we could not successfully get an input stream, then leave.
	if (!is)
	{
		cout << "Could not successfully acquire an input stream." << endl;
		return 1;
	}

	// Read the information.
	ReadInformation(is, vs, vf);

	// Process the information only if the sizes of the containers match.
	if (vs.size() == vf.size())
	{
		// Because inner_product is a templated function, the type of something is interpreted as is.
		// This means that "" is not interpreted as a string, but as a const char *.
		// Where necessary, types must match.  The type of the beginning and ending iterator of the
		// first collection will determine the data type of the first parameter for both binary operations.
		// The type of the beginning iterator for the second collection will determine the data type
		// of the second parameter for both binary operations.
		cout << inner_product(
			vs.begin(), vs.end(),	// The beginning and end of the first collection; size is determined by the end of the first collection.
			vf.begin(),				// The beginning of the second collection
			string(),				// The initial value for accumulation.  string() gives me an unnamed temporary representing a default string, i.e. an empty string.
			AddData,				// The "accumulation" operator.
			MultiplyData)			// The "multiplication" operator.
			<< endl;
	}
	else
	{
		cout << "Failed in trying to gather the information.";
	}

	// Close the file if we need to.
	CloseInputStream(is);

	// Return to OS
	return 0;
}

// FloatToString //////////////////////////////////////////
string FloatToString(float f)
{
	stringstream ss;

	// stringstreams are the best.  No explanations necessary,
	// this should be straightforward.
	ss << setprecision(2) << fixed;
	ss << f;

	// Return the generated string.
	return ss.str();
}

// ReadInformation ////////////////////////////////////////
void ReadInformation(istream &is, vector<string> &vs, vector<float> &vf)
{
	string s;
	float f;

	// If we are reading from the keyboard, give the user instructions
	// on how to end.
	if (&is == &cin)
	{
		cout << "Enter your data.  Please hit Ctrl-Z or F6 to end processing." << endl << endl;
	}

	// Read the information from the keyboard or file.
	while (is)
	{
		is >> s;
		if (!is.fail())
		{
			vs.push_back(s);	// Add it to the collection if it was read successfully.
		}

		is >> f;
		if (!is.fail())
		{
			vf.push_back(f);	// Add it to the collection if it was read successfully.
		}
	}
}

// GetInputStream /////////////////////////////////////////
istream &GetInputStream(void)
{
	static istream *is = NULL;
	static ifstream fin;
	static ifstream dummy(tmpnam(NULL));	// tmpnam() gives a filename that does not exist.

	string s;

	// Check to see if the istream variable is associated with a stream.
	// If it's not, go ahead and initialize it.
	if (!is)
	{
		// Find out where the user wants to get the information from.
		do
		{
			cout << "Read the information from the (K)eyboard, or from a (F)ile (k/F)? ";
			getline(cin, s);
		} while (s.length() < 1);

		// If the user chooses to read the information from the keyboard
		if (tolower(s[0]) == 'k')
		{
			is = &cin;
		}

		// If the user chooses to read the information from a file
		else
		{
			// Get the file name
			cout << "Enter the name of the input file: ";
			getline(cin, s);

			// Open the file
			fin.open(s.c_str());
			if (fin.is_open())
			{
				is = &fin;
			}

			// Okay, must bomb the program.  Return a reference to
			// an ifstream whose .fail() member function is always true.
			else
			{
				is = &dummy;
			}
		}
	}

	// Return the appropriate reference.
	return *is;
}

// CloseInputStream ///////////////////////////////////////
void CloseInputStream(istream &is)
{ // Close the file if the input stream is associated with a file.
	ifstream *infile = dynamic_cast<ifstream *>(&is);

	if (infile)
	{
		infile->close();
	}
}

// AddData ////////////////////////////////////////////////
string AddData(string s1, string s2)
{
	return s1 + s2;	// Simply join the strings together.
}

// MultiplyData ///////////////////////////////////////////
string MultiplyData(string s, float f)
{ // Where all of the magic happens.
	return s + ": $" + FloatToString(f) + '\n';
}