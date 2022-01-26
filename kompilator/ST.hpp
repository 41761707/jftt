#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <vector>

long long offset=0;

enum symtype {VARIABLE,ARRAY,ITER};

int tableErrors=0;

struct symrec
{
    std::string name;
    long long int length;
    long long int firstIndex;
    bool isInitialized;
    long long int position;
    symtype type;
};

typedef struct symrec symrec;

std::vector<symrec> sym_tab;

void  putsym (std::string sym_name, long long length, long long firstIndex, symtype type)
{
	symrec ptr;
	ptr.name=sym_name;
	ptr.length=length;
	ptr.firstIndex=firstIndex;
	ptr.isInitialized=false;
	ptr.position=offset;
	offset=offset+length;
	ptr.type=type;
	sym_tab.push_back(ptr);
	
}

int  getsym (std::string sym_name)
{
	for(int i=0;i<sym_tab.size();i++)
	{
		if(sym_tab[i].name==sym_name)
		{
			return i;
		}
	}
	return -1;
}
void init_var ( std::string sym_name, int lineno )
{
	int s=-1;
	s = getsym (sym_name);
	if (s == -1)
	{
		putsym(sym_name,1,1,VARIABLE);
	}
	else 
	{ 
		std::cout << sym_name << ": druga deklaracja, numer linii:" << lineno << std::endl;
		tableErrors=tableErrors+1;
	}
}
void init_iter(std::string sym_name, int lineno)
{
	int s=-1;
	s = getsym (sym_name);
	if (s == -1)
	{
		putsym(sym_name,1,1,ITER);
	}
	else 
	{ 
		std::cout << sym_name << ": druga deklaracja, numer linii:" << lineno << std::endl;
		tableErrors=tableErrors+1;
	}
}
void init_array(std::string sym_name, long long firstIndex,long long lastIndex, int lineno)
{
	int s=-1;
	s=getsym(sym_name);
	if(s!=-1)
	{
		std::cout << sym_name << ": druga deklaracja, numer linii:" << lineno << std::endl;
		tableErrors=tableErrors+1;
	}
	else if (firstIndex>lastIndex)
	{
		std::cout << "Niewłaściwy zakres tablicy: " << sym_name << ", numer linii: " << lineno << std::endl;
		tableErrors=tableErrors+1;
	}
	else
	{
		putsym(sym_name, (lastIndex - (firstIndex - 1)), firstIndex, ARRAY);
	}
}
bool symbol_exists(std::string sym_name)
{
	int s=-1;
	s=getsym(sym_name);
	if(s==-1)
	{
		return false;
	}
	else
	{
		return true;
	}
}
long long get_offset()
{
	return offset;
}

void set_init(std::string sym_name)
{
	int s;
	s=getsym(sym_name);
	sym_tab[s].isInitialized=true;
}

bool is_init(std::string sym_name)
{
	int s;
	s=getsym(sym_name);
	return sym_tab[s].isInitialized;
}
int arrayIndex(std::string sym_name)
{
	int s = getsym (sym_name);
	return sym_tab[s].firstIndex;
}