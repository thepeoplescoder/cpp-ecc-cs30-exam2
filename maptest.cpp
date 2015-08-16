#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <utility>

using namespace std;

int main(void)
{
	map<int, char> mymap;
	char ch = 'a';

	srand((unsigned int)time(NULL));

	for (int index = 0; index < 10; index++)
	{
		mymap[rand() % 200] = ch;
		ch++;
	}

	pair<map<int, char>::iterator, bool> ret;
	ret = mymap.insert(make_pair((int)(rand()%200), ch));
	if (ret.second)
	{
		cout << "successful insert." << endl;
	}

	for (map<int, char>::iterator iter = mymap.begin(); iter != mymap.end(); iter++)
	{
		cout << iter->first << '\t' << iter->second << endl;
	}

	return 0;
}