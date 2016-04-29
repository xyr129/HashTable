#include"HashTable.h"

void TestHash()
{
	/*HashTable<int> ht1(10);
	ht1.Insert(18);
	ht1.Insert(59);
	ht1.Insert(10);
	ht1.Insert(38);
	ht1.Insert(49);

	ht1.Print();

	cout << ht1.Remove(18) << endl;
	ht1.Print();

	cout << ht1.Remove(38) << endl;
	ht1.Print();*/

	HashTables<string, string> ht2(10);
	ht2.Insert("相交","cross");
	KeyValueNodes<string, string>* ret = ht2.Find("相交");
	if (NULL != ret)
	{
		ret->_value = "Cross";
	}
}

int main()
{
	TestHash();
	system("pause");
	return 0;
}