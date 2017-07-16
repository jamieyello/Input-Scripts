#pragma once
#include <iostream>
#include <string>
using namespace std;

namespace InputScripts {
	class HashTable
	{
	private:
		static const int table_size = 10000;

		struct item {
			string name;
			double val;
			item* next;
		};

		item* HashT[table_size];
		int Hashtable(string key);

	public:
		HashTable();
		~HashTable();
		
		void AddItem(string name, double val);
		int NumberOfItemsInIndex(int index);
		double FindVal(string name);
		void RemoveItem(string name);
	};
}