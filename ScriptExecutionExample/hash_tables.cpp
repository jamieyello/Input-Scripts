#include "stdafx.h"
#include "hash_tables.h"
#include <iostream>
#include <string>
using namespace std;

namespace InputScripts {
	//Goes through the table and sets the default values.
	HashTable::HashTable() {
		for (int i = 0; i < table_size; i++) {
			HashT[i] = new item;
			HashT[i]->name = "empty";
			HashT[i]->val = 0;
			HashT[i]->next = NULL;
		}
	}

	void HashTable::AddItem(string name, double val) {
		int index = Hashtable(name);
		if (HashT[index]->name == "empty") {
			HashT[index]->name = name;
			HashT[index]->val = val;
		}
		else {
			item* Ptr = HashT[index];
			item* n = new item;
			n->name = name;
			n->val = val;
			n->next = NULL;

			while (Ptr->next != NULL) {
				Ptr = Ptr->next;
			}
			Ptr->next = n;
		}
	}

	int HashTable::NumberOfItemsInIndex(int index) {
		int count = 0;

		if (HashT[index]->name == "empty") {
			return count;
		}
		else {
			count++;
			item* Ptr = HashT[index];
			while (Ptr->next != NULL) {
				count++;
				Ptr = Ptr->next;
			}
		}
		return count;
	}

	// Finds the hashtable to look for the value in.
	int HashTable::Hashtable(string key)
	{
		int hash = 0;
		int index;
		for (int i = 0; i < key.length(); i++) {
			hash += (int)key[i];
		}

		index = hash % table_size;

		return index;
	}

	double HashTable::FindVal(string name) {
		int index = Hashtable(name);
		bool foundName = false;
		double val = 0;

		item* Ptr = HashT[index];
		while (Ptr != NULL) {
			if (Ptr->name == name) {
				foundName = true;
				val = Ptr->val;
			}
			Ptr = Ptr->next;
		}

		return val;
	}

	void HashTable::RemoveItem(string name) {
		int index = Hashtable(name);

		item* delPtr;
		item* P1;
		item* P2;

		// Case 0 - bucket is empty
		if (HashT[index]->name == "empty" && HashT[index]->val == 0) {
		}

		// Case 1 - only 1 item contained in bucket and the item has a matching name
		else if (HashT[index]->name == name && HashT[index]->next == NULL) {
			HashT[index]->name = "empty";
			HashT[index]->val = 0;
		}

		// Case 2 - match is located in the first item in the bucket but there are more items in the bucket
		else if (HashT[index]->name == name) {
			delPtr = HashT[index];
			HashT[index] = HashT[index]->next;
			delete delPtr;
		}


		// Bucket contains items but first item is not a match
		else {
			P1 = HashT[index]->next;
			P2 = HashT[index];

			while (P1 != NULL && P1->name != name) {
				P2 = P1;
				P1 = P1->next;
			}
			// Looked through all the items in the bucket but there is no match
			if (P1 == NULL) {
			}
			// Match is found
			else {
				delPtr = P1;
				P1 = P1->next;
				P2->next = P1;

				delete delPtr;
			}
		}
	}

	HashTable::~HashTable()
	{
		item* Ptr;
		for (int i = 0; i < table_size; i++)
		{
			while (HashT[i] != NULL)
			{
				Ptr = HashT[i];
				HashT[i] = HashT[i]->next;
				delete Ptr;
			}
		}
	}
}