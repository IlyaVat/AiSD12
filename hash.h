#pragma once

class List
{
public:
	int value;
	List *next;
};

class Hash
{
private:
	List **cell;
	const int size;
	const int power;
	int modValue;
public:
	Hash();
	//~Hash();
	void outHash();
	int func(int);
};

