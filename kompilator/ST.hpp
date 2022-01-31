#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <vector>

/* Zmienna przechowująca informację o długości przechowywanych zmiennych */
long long offset=0;

/* Typ wyliczeniowy zawietający możliwe typy zmiennych 
*	VARIABLE- zmienna
*	ARRAY- zmienna tablicowa
*	ITER- iterator pętli
*/
enum symtype {VARIABLE,ARRAY,ITER};

/* Zmienna przechowująca informację o liczbie występujących błędów w inicjacji zmiennych */
int tableErrors=0;


/* Struktura przechowująca informacje na temat pojedynczej zmiennej 
*  name- nazwa zmiennej
*  length- długośc zmiennej (dla zwykłych zmiennych stale równa 1, dla zmiennych tablicowych równa liczbie elementów tablicy)
*  firstIndex- pierwszy indeks (dotyczy zmiennej tablicowej)
*  isInitialized- czy aktualna zmienna jest zainicjalizowana
*  position- pozycja w tablicy symboli
*  type- typ zmiennej (pochodzący z typu wyliczeniowego symtype)
*/
struct symrec
{
    std::string name;
    long long length;
    long long firstIndex;
    bool isInitialized;
    long long position;
    symtype type;
};

typedef struct symrec symrec;

/*Wektor przechowujący zmienne zadeklarowane w programie */
std::vector<symrec> sym_tab;

/* Funkcja odpowiedzialna za utworzenie nowego symbolu 
*	@param sym_name- nazwa symbolu
*	@param length- długość zmiennej
*	@param firstIndex- pierwszy indeks tablicy
*	@param type- typ zmiennej
*/
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
/* Funkcja zwracająca pozycję zmiennej w wektorze zmiennych o podanej nazwie
	@param sym_name- nazwa poszukiwanej zmiennej
	@return i- pozycja zmiennej w wektorze zmiennych, -1, gdy takowej zmiennej nie ma
*/
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
/* 	Funkcja inicjująca zmienną
	@param sym_name- nazwa zmiennej
	@param lieno- aktualny numer linii
*/
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
/* 	Funkcja inicjująca iterator
	@param sym_name- nazwa iteratora
	@param lieno- aktualny numer linii
*/
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
/* 	Funkcja inicjująca tablicy
	@param sym_name- nazwa tablicy
	@param lieno- aktualny numer linii
*/
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
/* Funkcja sprawdzająca, czy zmienna istnieje 
*  @param sym_name- nazwa poszukiwanej zmiennej 
*/
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

/* Funkcja inicjalizująca zmienną
*	@param sym_name- nazwa zmiennej
*/
void set_init(std::string sym_name)
{
	int s;
	s=getsym(sym_name);
	sym_tab[s].isInitialized=true;
}

/* Funkcja sprawdzająca, czy zmienna jest zainicjalizowana
*	@param sym_name- nazwa zmiennej
*/
bool is_init(std::string sym_name)
{
	int s;
	s=getsym(sym_name);
	return sym_tab[s].isInitialized;
}
/* Funkcja zwracająca pierwszy indeks tablicy
*	@param sym_name- nazwa zmiennej
*/
int arrayIndex(std::string sym_name)
{
	int s = getsym (sym_name);
	return sym_tab[s].firstIndex;
}