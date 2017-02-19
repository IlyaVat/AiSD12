#include "stdafx.h"
#include "hash.h"
#include <iomanip>
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

int Hash::func(int x)
{
	return (97*x+11) % modValue;
}

Hash::Hash(): size(11), power(26)
{
	int key;
	int adress;
	cell = new List*[size];
	modValue = size;
	for (int i=0; i<size; i++)
	{
		cell[i] = new List;
		cell[i]->next = nullptr;
		cell[i]->value = -1;
	}
	for (int i=0; i<power; i++)
	{
		key = rand()%101;
		adress = func(key);
		cout<<adress<<" ";//<<"\n"<<modValue;
		//_getch();
		if (cell[adress]->value==-1)
			cell[adress]->value=key;
		else
		{
			List *newObj = new List;
			newObj->value = key;
			newObj->next = nullptr;
			List *temp = cell[adress];
			while (temp->next!=nullptr)
				temp = temp->next;
			temp->next = newObj;
		}
	}
	cout<<endl;
}

void Hash::outHash()
{
	for (int i=0; i<size; i++)
	{
		cout<<"_____________________________\n";
		cout<<"| "<<i<<" | ";
		List *temp=cell[i];
		while(temp!=nullptr)
		{
			cout<<temp->value;
			if (temp->next!=nullptr)
				cout<<" -> ";
			temp = temp->next;
		}
		cout<<"\n";
	}
}


/*Hash::~Hash()
{
	for (int i=0; i<size; i++)
		delete [] cell[i];
	delete [] cell;
}*/

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(nullptr));
	Hash A;
	A.outHash();
	_getch();
	return 0;
}