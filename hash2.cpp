#include "stdafx.h"
#include "hash2.h"
#include <iomanip>
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

int Hash::func(int x)
{
	return (97 * x + 11) % modValue;
}

void Hash::Add(int key)
{
	int adress = func(key);
	if (cell[adress]->value == -1)
		cell[adress]->value = key;
	else
	{
		List *newObj = new List;
		newObj->value = key;
		newObj->next = this->cell[adress];
		this->cell[adress] = newObj;
	}
}

Hash::Hash()
{
	int key;
	int adress;
	bool repeat = 0;
	cell = new List*[size];
	modValue = size;
	for (int i = 0; i<size; i++)
	{
		cell[i] = new List;
		cell[i]->next = nullptr;
		cell[i]->value = -1;
	}
	for (int i = 0; i<power; i++)
	{
		do
		{
			repeat = 0;
			key = rand() % 101;
			adress = func(key);
			cout << adress << " ";
			if (cell[adress]->value == -1)
				cell[adress]->value = key;
			else
				repeat = 1;
		} while (repeat);
	}
}

Hash::Hash(int x)
{
	int key;
	int adress;
	bool repeat = 0;
	cell = new List*[size];
	modValue = size;
	for (int i = 0; i<size; i++)
	{
		cell[i] = new List;
		cell[i]->next = nullptr;
		cell[i]->value = -1;
	}
}

Hash::~Hash()
{
	for (int i = 0; i < size; i++)
	{
		List *temp = cell[i];
		while (temp->next)
		{
			List *current = temp;
			temp = temp->next;
			delete[] current;
		}
	}
	delete [] cell;
}

Hash &Hash::operator | (const Hash & B)const
{
	bool canAdd = 1;
	Hash *C = new Hash(0);
	for (int i = 0; i < size; i++)
	{
		List *temp = this->cell[i];
		while (temp != nullptr)
		{
			C->Add(temp->value);
			temp = temp->next;
		}
		temp = B.cell[i];
		while (temp != nullptr)
		{
			canAdd = 1;
			List *tempC = C->cell[i];
			while (tempC != nullptr)
			{
				if (temp->value == tempC->value)
					canAdd = 0;
				tempC = tempC->next;
			}
			if (canAdd)
				C->Add(temp->value);
			temp = temp->next;
		}
	}
	return *C;
}

Hash &Hash::operator / (const Hash & B)const
{
	Hash *C = new Hash(0);
	for (int i = 0; i < size; i++)
	{
		List* xB = B.cell[i];
		List* xA = this->cell[i];
		List *temp;
		while (xA)
		{
			temp = xB;
			bool flag = 1;
			while (temp&&flag)
			{
				if (xA->value == temp->value)
					flag = 0;
				temp = temp->next;
			}
			if (flag)
				C->Add(xA->value);
			xA = xA->next;
		}
	}

	return *C;
}

Hash &Hash::operator ^ (const Hash & B)const
{
	Hash *C = new Hash(0);
	for (int i = 0; i < size; i++)
	{
		List* xB = B.cell[i];
		List* xA = this->cell[i];
		while (xB)
		{
			bool canAdd = 1;
			xA = this->cell[i];
			while (xA)
			{
				if (xB->value == xA->value)
					canAdd = 0;
				xA = xA->next;
			}
			if (canAdd)
				C->Add(xB->value);
			xB = xB->next;
		}
		xA = this->cell[i];
		while (xA)
		{
			bool canAdd = 1;
			List* xB = B.cell[i];
			while (xB)
			{
				if (xA->value == xB->value)
					canAdd = 0;
				xB = xB->next;
			}
			if (canAdd)
				C->Add(xA->value);
			xA = xA->next;
		}

	}
	return *C;
}

void Hash::operator = (const Hash & B)
{

	for (int i = 0; i < size; i++)
	{
		List *temp = B.cell[i];
		while (temp != nullptr)
		{
			this->Add(temp->value);
			temp = temp->next;
		}
	}
}

void Hash::outHash()
{
	system("cls");
	for (int i = 0; i<size; i++)
	{
		cout << "_____________________________\n";
		cout << "| " << i << " | ";
		List *temp = cell[i];
		while (temp != nullptr)
		{
			cout << temp->value;
			if (temp->next != nullptr)
				cout << " -> ";
			temp = temp->next;
		}
		cout << "\n";
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(nullptr));
	Hash A, B, C, D, E, F(0), G(0), H;
	//A.outHash();
	//_getch();
	//B.outHash();
	//_getch();
	F = A|B|C^D/E;
	F.outHash();
	_getch();
	return 0;
}