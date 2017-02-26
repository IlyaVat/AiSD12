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
	const int size=26;
	const int power=26;
	int modValue;
public:
	Hash();
	Hash(int);
	~Hash();
	Hash &operator | (const Hash&)const;
	Hash &operator / (const Hash&)const;
	Hash &operator ^ (const Hash&)const;
	void operator  = (const Hash&);
	void outHash();
	void Add(int);
	int func(int);
};


