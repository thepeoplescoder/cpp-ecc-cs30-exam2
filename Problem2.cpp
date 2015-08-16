/*
	Austin Cathey
	Computer Science 30
	Exam 2
	Problem 2
*/

// Includes
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <fstream>

// Use standard namespace
using namespace std;

// Typedefs
typedef pair<string, float> Donation;
typedef multimap<string, float> Donations;
typedef set<string> GroupNames;

// Constants
const float MAX_DONATION = 99999.99f;

// Prototypes
bool read_from_keyboard(void);
istream &acquire_istream(bool keyboard);
void close_istream(istream &is);
bool write_to_screen(void);
ostream &acquire_ostream(bool screen);
void close_ostream(ostream &os);
void read_groups(istream &is, Donations &data, GroupNames &group_names);
void write_groups(ostream &os, Donations &donations, GroupNames &group_names);

// Main
int main(void)
{
	Donations donations;
	GroupNames group_names;

	// Get an input stream.
	istream &is = acquire_istream(read_from_keyboard());
	if (!is)
	{
		cout << "Could not get an input stream." << endl;
		return 1;
	}

	// Get an input stream.
	ostream &os = acquire_ostream(write_to_screen());
	if (!os)
	{
		cout << "Could not get an output stream." << endl;
		return 1;
	}

	// Get the information, then write it.
	read_groups(is, donations, group_names);
	write_groups(os, donations, group_names);

	// Close any files associated with the stream.
	close_istream(is);
	close_ostream(os);

	// Return to OS
	return 0;
}

// read_from_keyboard /////////////////////////////////////
bool read_from_keyboard(void)
{
	string s;

	cout << "Read from the (k)eyboard or a (f)ile (k/F)? ";
	getline(cin, s);
	s += 'F';			// Ensure that the length of the string is at least one character.

	return s[0] == 'k' || s[0] == 'K';
}

// acquire_istream ////////////////////////////////////////
istream &acquire_istream(bool keyboard)
{
	static istream *is = NULL;
	static ifstream fin;
	static string filename;

	// Get the stream if we have not done so already.
	if (!is)
	{
		// If we are going to use the keyboard.
		if (keyboard)
		{
			is = &cin;
		}

		// If we are going to use a file.
		else
		{
			cout << "Enter the name of the input file: ";	// Ask user for filename
			getline(cin, filename);							// Get filename
			fin.open(filename.c_str());						// Attempt to open filename
			is = &fin;										// Get address of stream
		}
	}

	return *is;
}

// close_istream //////////////////////////////////////////
void close_istream(istream &is)
{
	ifstream *fin = dynamic_cast<ifstream *>(&is);

	if (fin)
	{
		fin->close();
	}
}

// write_to_screen ////////////////////////////////////////
bool write_to_screen(void)
{
	string s;

	cout << "Write to the (s)creen or to a (f)ile (s/F)? ";
	getline(cin, s);
	s += 'F';			// Ensure that the length of the string is always at least one.

	return s[0] == 's' || s[0] == 'S';
}

// acquire_ostream ////////////////////////////////////////
ostream &acquire_ostream(bool screen)
{
	static ostream *os = NULL;
	static ofstream fout;
	string filename;

	// If the output stream has not been initialized, then initialize it.
	if (!os)
	{
		// Use the screen.
		if (screen)
		{
			os = &cout;
		}

		// Use an output file.
		else
		{
			cout << "Enter output file name: ";
			getline(cin, filename);
			fout.open(filename.c_str());
			os = &fout;
		}
	}

	// Return the output stream.
	return *os;
}

// close_ostream //////////////////////////////////////////
void close_ostream(ostream &os)
{
	ofstream *fout = dynamic_cast<ofstream *>(&os);

	if (fout)
	{
		fout->close();
	}
}

// read_groups ////////////////////////////////////////////
void read_groups(istream &is, Donations &data, GroupNames &group_names)
{
	string group_name;
	float amount;

	// Keep reading information as long as everything is fine with the stream.
	while (is)
	{
		// Read the name of the group.  If successful, move on.
		if (getline(is, group_name))
		{
			// Get the amount, and continue if successful.
			if (is >> amount)
			{
				// Get rid of the leftover newline.
				getline(is, string());

				// Save the information, since retrieving it was successful.
				data.insert(Donation(group_name, amount));
				group_names.insert(group_name);				// An insert is okay here, because duplicate names are ignored.
			}
		}
	}
}

// write_groups ///////////////////////////////////////////
void write_groups(ostream &os, Donations &donations, GroupNames &group_names)
{
	GroupNames::iterator names;										// To get the names of the groups.
	pair<Donations::iterator, Donations::iterator> donations_iters;	// To iterate through each donation.
	float sum;														// The total sum of donations to a particular group.

	// Go through every group name.
	for (names = group_names.begin(); names != group_names.end(); names++)
	{
		// Get every element that belongs to the group.  equal_range() will give me
		// a pair of iterators of the type of the container variable that I'm working with.
		// Skipping past many technical details, the thing to point out here is that if
		// the name doesn't exist, the pair of iterators returned should give me an empty
		// range. (i.e. both iterators will be the same.)
		//
		// The first iterator will be the beginning iterator.
		// The second iterator will be the ending iterator.
		//
		donations_iters = donations.equal_range(*names);

		// Attempt to gather the information.
		if (donations_iters.first != donations_iters.second)
		{
			// Reset the sum to zero since we're getting a total donation.
			sum = 0.0f;

			// Go through every donation belonging to the group.
			for (Donations::iterator iter = donations_iters.first;	// First item belonging to group
				iter != donations_iters.second;						// donations_iters.second is the ending iterator.
				iter++)												// Keep advancing to the next iterator.
			{
				sum += iter->second;		// Add the donation amount.  Remember, in this case, iter points to an actual item
											// in the multimap.  Accessing the multimap this way gives us access to the key and
											// the value, and in this case, we only want the value, which is the second item in
											// the pair.

				if (sum > MAX_DONATION)		// Make sure the sum doesn't exceed the maximum.
				{
					sum = MAX_DONATION;
				}
			}

			// Output the information.
			os << *names << endl;
			os << sum << endl;
		}
	}
}