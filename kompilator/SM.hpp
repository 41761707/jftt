#pragma once
#include "ST.hpp"
#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <vector>

int codeErrors=0;
std::vector<std::string> command_list;


struct registers
{
	std::string registerName;
};

enum var_type
{
    VAL,
    _VAR,
    ARR,
    ARRIND,
    EXPR
};

struct variable
{
    std::string name;
    std::string index;
    long long value;
    var_type type;
};
typedef struct variable var;

std::vector<variable>iterators;
struct condition
{
    std::string name;
    long long pre_cond_index;
    long long post_cond_index;
};
typedef struct condition condition;

std::vector<condition> condition_start;

struct forLabel
{
	std::string name;
	long long startValue;
	long long stopValue;
	long long index;

};
typedef struct forLabel forLabel;

std::vector<forLabel> forLabel_start;

typedef struct registers registers;
registers registersTable[8];


void init_registers()
{
	registersTable[0].registerName="a";
	registersTable[1].registerName="b";
	registersTable[2].registerName="c";
	registersTable[3].registerName="d";
	registersTable[4].registerName="e";
	registersTable[5].registerName="f";
	registersTable[6].registerName="g";
	registersTable[7].registerName="h";
	condition cond;
	condition_start.push_back(cond);
	forLabel forLabel;
	forLabel_start.push_back(forLabel);
}

void test_print()
{
	for(int i=0;i<sym_tab.size();i++)
	{
		std::cout << i << ", " << sym_tab[i].name << std::endl;
		std::cout << "Length: " << sym_tab[i].length << ", FirstIndex:" << sym_tab[i].firstIndex << std::endl;
		std::cout << "Position: " << sym_tab[i].position << std::endl;
	}
}
std::string decToBin(long long number)
{
	std::string result="";
	while(number>0)
	{
		if(number%2==1)
		{
			result=result+"1";
		}
		else
		{
			result=result+"0";
		}
		number=number/2;
	}
	return result;
}
/* COMMANDS */
/*----------------------------*/
void halt()
{
	command_list.push_back("HALT");
	//test_print();
}
void reset(registers &reg)
{
	command_list.push_back("RESET " + reg.registerName);
}
void reset_value_only(registers &reg)
{
	command_list.push_back("RESET " + reg.registerName);
}
void inc(registers &reg)
{
	command_list.push_back("INC " + reg.registerName);
}
void dec(registers &reg)
{
	command_list.push_back("DEC " + reg.registerName);
}
void get()
{
	command_list.push_back("GET");
}
void add(registers &reg)
{
	command_list.push_back("ADD " + reg.registerName);
}
void sub(registers &reg)
{
	command_list.push_back("SUB " + reg.registerName);
}
void swap(registers &reg)
{
	command_list.push_back("SWAP " + reg.registerName);
}
void put()
{
	command_list.push_back("PUT");
}
void load(registers &reg)
{
	command_list.push_back("LOAD " + reg.registerName);
}
void store(registers &reg)
{
	command_list.push_back("STORE " + reg.registerName);
}
void shift(registers &reg)
{
	command_list.push_back("SHIFT " + reg.registerName);
}
void jump(long long j)
{
	command_list.push_back("JUMP " + std::to_string(j));
}
void jpos(long long j)
{
	command_list.push_back("JPOS " + std::to_string(j));

}
void jzero(long long j)
{
	command_list.push_back("JZERO " + std::to_string(j));

}
void jneg(long long j)
{
	command_list.push_back("JNEG " + std::to_string(j));

}
/*----------------------------*/

var *func_num(long long int value, int lineno)
{
	var *current_var;
	current_var=new var;
	current_var->value=value;
	current_var->type=VAL;
	return current_var;
}

var *func_pid(std::string name, int lineno)
{
	var *current_var;
	current_var=new var;
	current_var->name=name;
	current_var->value=(long long)0;
	current_var->type=_VAR;
	return current_var;
}

var *func_pid_arr(std::string name, std::string index, int lieno)
{
	var *current_var;
	current_var=new var;
	current_var->name=name;
	current_var->type=ARRIND;
	current_var->value=(long long )1;
	current_var->index=index;
	return current_var;

}

var *func_pid(std::string name, long long index, int lineno)
{
	var *current_var;
	current_var=new var;
	current_var->name=name;
	current_var->index="";
	current_var->type=ARR;  //TU UWAGA
	current_var->value=index;
	return current_var;

}

void generateConst(long long value)
{
	if(value<0)
		{
			long long a=value;
			std::string number=decToBin(value*(-1));
			int current_position=number.size()-1;
			reset(registersTable[1]);
			reset(registersTable[2]);
			dec(registersTable[0]);
			for(int i=0;i<number.size();i++)
			{
				inc(registersTable[1]);
			}
			dec(registersTable[1]);
			shift(registersTable[1]);
			swap(registersTable[2]);
			for(int i=number.size()-2;i>=0;i--)
			{
				if(number[i]=='1')
				{
					dec(registersTable[0]);
					for(int a=current_position-i;a>0;a--)
					{
						dec(registersTable[1]);
					}
					current_position=i;
					shift(registersTable[1]);
					add(registersTable[2]);
					swap(registersTable[2]);
					reset(registersTable[0]);
				}
			}
		}
		else
		{
			int a=value;
			std::string number=decToBin(value);
			int current_position=number.size()-1;
			reset(registersTable[1]);
			reset(registersTable[2]);
			inc(registersTable[0]);
			for(int i=0;i<number.size();i++)
			{
				inc(registersTable[1]);
			}
			dec(registersTable[1]);
			shift(registersTable[1]);
			swap(registersTable[2]);
			for(int i=number.size()-2;i>=0;i--)
			{
				if(number[i]=='1')
				{
					inc(registersTable[0]);
					for(int a=current_position-i;a>0;a--)
					{
						dec(registersTable[1]);
					}
					current_position=i;
					shift(registersTable[1]);
					add(registersTable[2]);
					swap(registersTable[2]);
					reset(registersTable[0]);
				}
			}
		}
		swap(registersTable[2]);
}

/*GRAMMAR FUNCTIONS */
/*----------------------------*/
void read(var *name, int lineno)
{
	reset(registersTable[0]);
	if(name->type==_VAR)
	{
		int i=getsym(name->name);
		generateConst(sym_tab[i].position);
		sym_tab[i].isInitialized=true;
	}
	else if(name->type==ARRIND)
	{
		reset(registersTable[6]);
		int i=getsym(name->name);
		int index=getsym(name->index);
		reset(registersTable[0]);
		generateConst(sym_tab[i].position);
		swap(registersTable[6]);
		reset(registersTable[0]);
		generateConst(sym_tab[index].position);
		reset(registersTable[7]);
		swap(registersTable[7]);
		load(registersTable[7]);
		swap(registersTable[7]);
		reset(registersTable[0]);
		int array=arrayIndex(name->name);
		generateConst(array);
		swap(registersTable[5]);
		reset(registersTable[0]);
		add(registersTable[6]);
		add(registersTable[7]);
		sub(registersTable[5]);
	}
	else if(name->type==ARR)
	{
		int i=getsym(name->name);
		int index=name->value-sym_tab[i].firstIndex;
		index=sym_tab[i].position+index;
		generateConst(index);
	}
	reset(registersTable[1]);
	swap(registersTable[1]);
	load(registersTable[1]);
	get();
	store(registersTable[1]);

}
void write(var *current, int lineno)
{
	reset(registersTable[0]);
	reset(registersTable[1]);
	if(current->type==VAL)
	{
		reset(registersTable[0]);
		generateConst(current->value);
		put();
	}
	else if(current->type==_VAR)
	{
		int i=getsym(current->name);
		if(i==-1)
		{
			std::cout << "Niezadeklarowana zmienna: " << current->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		if(sym_tab[i].isInitialized==false)
		{
			std::cout << "Użycie niezainicjowanej zmiennej: " << current->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		reset(registersTable[0]);
		generateConst(sym_tab[i].position);
		swap(registersTable[1]);
		load(registersTable[1]);
		put();
	}
	else if(current->type==ARR)
	{
		int i=getsym(current->name);
		int index=current->value-sym_tab[i].firstIndex;
		index=sym_tab[i].position+index;
		reset(registersTable[0]);
		generateConst(index);
		swap(registersTable[1]);
		load(registersTable[1]);
		put();

	}
	else if(current->type==ARRIND)
	{
		reset(registersTable[6]);
		int i=getsym(current->name);
		int index=getsym(current->index);
		reset(registersTable[0]);
		generateConst(sym_tab[i].position);
		swap(registersTable[6]);
		reset(registersTable[0]);
		generateConst(sym_tab[index].position);
		reset(registersTable[7]);
		swap(registersTable[7]);
		load(registersTable[7]);
		swap(registersTable[7]);
		reset(registersTable[0]);
		int array=arrayIndex(current->name);
		generateConst(array);
		swap(registersTable[5]);
		reset(registersTable[0]);
		add(registersTable[6]);
		add(registersTable[7]);
		sub(registersTable[5]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load(registersTable[1]);
		put();

	}
}
void assign(var *variable, var *expr, int lineno)
{
	reset(registersTable[0]);
	if(variable->type==_VAR)
	{
		reset(registersTable[6]);
		int i=getsym(variable->name);
		generateConst(sym_tab[i].position);
		if(sym_tab[i].type==2 && sym_tab[i].isInitialized==true)
		{
			std::cout << "Zabroniona modyfikacja iteratora: " << variable->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		sym_tab[i].isInitialized=true;
		if(sym_tab[i].type==1)
		{
			std::cout << "Niewłaściwe użycie zmiennej tablicowej: " << variable->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		swap(registersTable[6]);
		reset(registersTable[0]);
	}
	else if(variable->type==ARR)
	{
		reset(registersTable[6]);
		int i=getsym(variable->name);
		if(sym_tab[i].type==0)
		{
			std::cout << "Niewłaściwe użycie zmiennej: " << variable->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		int index=variable->value-sym_tab[i].firstIndex;
		index=sym_tab[i].position+index;
		generateConst(index);
		swap(registersTable[6]);	
	}
	else if(variable->type==ARRIND)
	{	
		reset(registersTable[6]);
		int i=getsym(variable->name);
		if(sym_tab[i].type==0)
		{
			std::cout << "Niewłaściwe użycie zmiennej: " << variable->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		int index=getsym(variable->index);
		if(index==-1)
		{
			std::cout << "Niezadeklarowana zmienna: " << variable->index << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		reset(registersTable[0]);
		generateConst(sym_tab[i].position);
		swap(registersTable[6]);
		reset(registersTable[0]);
		generateConst(sym_tab[index].position);
		reset(registersTable[7]);
		swap(registersTable[7]);
		load(registersTable[7]);
		swap(registersTable[7]);
		reset(registersTable[0]);
		int array=arrayIndex(variable->name);
		generateConst(array);
		swap(registersTable[4]);
		reset(registersTable[0]);
		add(registersTable[6]);
		add(registersTable[7]);
		sub(registersTable[4]);
		swap(registersTable[6]);
		reset(registersTable[0]);
	}
	if(expr->type==EXPR)
	{
		reset(registersTable[0]);
		add(registersTable[5]);
		store(registersTable[6]);
	}
	else if(expr->type==VAL)
	{
		generateConst(expr->value);
		store(registersTable[6]);
	}
	else if(expr->type==_VAR)
	{
		int newi=getsym(expr->name);
		if(newi==-1)
		{
			std::cout << "Niezadeklarowana zmienna: " << expr->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		if(sym_tab[newi].isInitialized==false)
		{
			std::cout << "Użycie niezainicjowanej zmiennej: " << expr->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		if(sym_tab[newi].type==1)
		{
			std::cout << "Niewłaściwe użycie zmiennej tablicowej: " << expr->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		generateConst(sym_tab[newi].position);
		swap(registersTable[1]);
		load(registersTable[1]);
		store(registersTable[6]);
	}
	else if(expr->type==ARRIND)
	{
		reset(registersTable[4]);
		int i=getsym(expr->name);
		if(sym_tab[i].type==0)
		{
			std::cout << "Niewłaściwe użycie zmiennej: " << expr->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		int index=getsym(expr->index);
		if(index==-1)
		{
			std::cout << "Niezadeklarowana zmienna: " << expr->index << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		reset(registersTable[0]);
		generateConst(sym_tab[i].position);
		swap(registersTable[4]);
		reset(registersTable[0]);
		generateConst(sym_tab[index].position);
		reset(registersTable[7]);
		swap(registersTable[7]);
		load(registersTable[7]);
		swap(registersTable[7]);
		reset(registersTable[0]);
		int array=arrayIndex(expr->name);
		generateConst(array);
		swap(registersTable[5]);
		reset(registersTable[0]);
		add(registersTable[4]);
		add(registersTable[7]);
		sub(registersTable[5]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load(registersTable[1]);
		store(registersTable[6]);
		
	}
	else if(expr->type==ARR)
	{
		reset(registersTable[5]);
		int i=getsym(expr->name);
		if(sym_tab[i].type==0)
		{
			std::cout << "Niewłaściwe użycie zmiennej: " << variable->name << ", numer linii: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		int index=expr->value-sym_tab[i].firstIndex;
		index=sym_tab[i].position+index;
		reset(registersTable[0]);
		generateConst(index);
		swap(registersTable[5]);
		load(registersTable[5]);
		store(registersTable[6]);
	}
}
void load_var(var *v1, var *v2, int lineno)
{
	reset(registersTable[0]);
	if(v1->type==_VAR)
	{
		int i=getsym(v1->name);
		if(i==-1)
		{
			std::cout << "Nierozpoznana zmienna: " << v1->name << ", linia: " << lineno << std::endl;
			codeErrors=codeErrors+1;
		}
		reset(registersTable[0]);
		generateConst(sym_tab[i].position);
		swap(registersTable[6]);
		load(registersTable[6]);
		swap(registersTable[6]);
	}
	else if(v1->type==VAL)
	{
		generateConst(v1->value);
		reset(registersTable[6]);
		swap(registersTable[6]);
	}
	else if(v1->type==ARRIND)
	{
		reset(registersTable[4]);
		int i=getsym(v1->name);
		int index=getsym(v1->index);
		reset(registersTable[0]);
		generateConst(sym_tab[i].position);
		swap(registersTable[4]);
		reset(registersTable[0]);
		generateConst(sym_tab[index].position);
		reset(registersTable[7]);
		swap(registersTable[7]);
		load(registersTable[7]);
		swap(registersTable[7]);
		reset(registersTable[0]);
		int array=arrayIndex(v1->name);
		generateConst(array);
		swap(registersTable[5]);
		reset(registersTable[0]);
		add(registersTable[4]);
		add(registersTable[7]);
		sub(registersTable[5]);
		reset(registersTable[6]);
		swap(registersTable[6]);
		load(registersTable[6]);
		swap(registersTable[6]);
	}
	else if(v1->type=ARR)
	{
		int i=getsym(v1->name);
		int index=v1->value-sym_tab[i].firstIndex;
		index=sym_tab[i].position+index;
		reset(registersTable[0]);
		generateConst(index);
		reset(registersTable[6]);
		swap(registersTable[6]);
		load(registersTable[6]);
		swap(registersTable[6]);
	}
	reset(registersTable[0]);
	if(v2->type==_VAR)
	{
		int i=getsym(v2->name);
		reset(registersTable[0]);
		generateConst(sym_tab[i].position);
		reset(registersTable[5]);
		swap(registersTable[5]);
		load(registersTable[5]);
	}
	else if(v2->type==VAL)
	{
		generateConst(v2->value);
	}
	else if(v2->type==ARRIND)
	{
		reset(registersTable[4]);
		int i=getsym(v2->name);
		int index=getsym(v2->index);
		reset(registersTable[0]);
		generateConst(sym_tab[i].position);
		swap(registersTable[4]);
		reset(registersTable[0]);
		generateConst(sym_tab[index].position);
		reset(registersTable[7]);
		swap(registersTable[7]);
		load(registersTable[7]);
		swap(registersTable[7]);
		reset(registersTable[0]);
		int array=arrayIndex(v2->name);
		generateConst(array);
		swap(registersTable[5]);
		reset(registersTable[0]);
		add(registersTable[4]);
		add(registersTable[7]);
		sub(registersTable[5]);
		reset(registersTable[5]);
		swap(registersTable[5]);
		load(registersTable[5]);
	}
	else if(v2->type=ARR)
	{
		int i=getsym(v2->name);
		int index=v2->value-sym_tab[i].firstIndex;
		index=sym_tab[i].position+index;
		reset(registersTable[0]);
		generateConst(index);
		reset(registersTable[5]);
		swap(registersTable[5]);
		load(registersTable[5]);
	}
}
void *func_plus( var *v1, var *v2, int lineno)
{
	load_var(v1,v2,lineno);
	add(registersTable[6]);
	reset(registersTable[5]);
	swap(registersTable[5]);
	v1->type=EXPR;
}
void *func_minus( var *v1, var *v2, int lineno)
{
	load_var(v1,v2,lineno);
	swap(registersTable[6]);
	sub(registersTable[6]);
	reset(registersTable[5]);
	swap(registersTable[5]);
	v1->type=EXPR;

}
void *func_times( var *v1, var *v2, int lineno)
{

	load_var(v1,v2,lineno);
	reset(registersTable[7]);
	jzero(52);
	jpos(5);
	dec(registersTable[7]);
	swap(registersTable[2]);
	reset(registersTable[0]);
	sub(registersTable[2]);
	swap(registersTable[2]);
	swap(registersTable[6]);
	jzero(44);
	jpos(5);
	inc(registersTable[7]);
	swap(registersTable[1]);
	reset(registersTable[0]);
	sub(registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[4]);
	reset(registersTable[3]);
	reset(registersTable[0]);
	inc(registersTable[0]);
	shift(registersTable[3]);
	sub(registersTable[2]);
	jpos(4);
	reset(registersTable[0]);
	inc(registersTable[3]);
	jump(-7);
	dec(registersTable[3]);
	reset(registersTable[0]);
	add(registersTable[1]);
	shift(registersTable[3]);
	swap(registersTable[4]);
	add(registersTable[4]);
	swap(registersTable[4]);
	reset(registersTable[0]);
	inc(registersTable[0]);
	shift(registersTable[3]);
	swap(registersTable[2]);
	sub(registersTable[2]);
	swap(registersTable[2]);

	reset(registersTable[0]);
	inc(registersTable[0]);
	dec(registersTable[3]);
	swap(registersTable[3]);
	jneg(7);
	swap(registersTable[3]);
	shift(registersTable[3]);
	sub(registersTable[2]);
	jneg(-20);
	jzero(-21);
	jump(-10);
	swap(registersTable[4]);
	swap(registersTable[7]);
	jzero(4);
	reset(registersTable[0]);
	sub(registersTable[7]);
	jump(2);
	swap(registersTable[7]);
	swap(registersTable[5]);
	jump(3);

	reset(registersTable[0]);
	swap(registersTable[5]);

	v1->type=EXPR;

}
void *func_div(var *v1, var *v2, int lineno)
{
	load_var(v1,v2,lineno);
	reset(registersTable[2]);
	reset(registersTable[1]);
	reset(registersTable[7]);
	reset(registersTable[5]);
	jzero(61);
	jpos(5);
	dec(registersTable[7]);
	swap(registersTable[2]);
	reset(registersTable[0]);
	sub(registersTable[2]);
	swap(registersTable[2]);
	swap(registersTable[6]);
	jzero(53);
	reset(registersTable[6]);
	jpos(6);
	inc(registersTable[6]);
	inc(registersTable[7]);
	swap(registersTable[1]);
	reset(registersTable[0]);
	sub(registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[4]);
	reset(registersTable[3]);
	reset(registersTable[0]);
	add(registersTable[2]);
	shift(registersTable[3]);
	sub(registersTable[1]);
	jpos(3);
	inc(registersTable[3]);
	jump(-6);
	dec(registersTable[3]);
	swap(registersTable[3]);
	jneg(42);
	swap(registersTable[3]);
	reset(registersTable[0]);
	inc(registersTable[0]);
	shift(registersTable[3]);
	swap(registersTable[4]);
	add(registersTable[4]);
	swap(registersTable[4]);
	reset(registersTable[0]);
	add(registersTable[2]);
	shift(registersTable[3]);
	swap(registersTable[1]);
	sub(registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[0]);
	add(registersTable[1]);
	sub(registersTable[2]);
	jneg(12);
	reset(registersTable[0]);
	add(registersTable[2]);
	shift(registersTable[3]);
	sub(registersTable[1]);
	jneg(-20);
	jzero(-21);
	dec(registersTable[3]);
	swap(registersTable[3]);
	jneg(3);
	swap(registersTable[3]);
	jump(-10);

	swap(registersTable[4]);
	swap(registersTable[7]);
	jzero(8);
	reset(registersTable[0]);
	sub(registersTable[7]);
	swap(registersTable[1]);
	jzero(2);
	dec(registersTable[1]);
	swap(registersTable[1]);
	jump(2);
	swap(registersTable[7]);
	swap(registersTable[5]);
	jump(3);

	reset(registersTable[0]);
	swap(registersTable[5]);
	v1->type=EXPR;
}
void *func_mod(var *v1, var *v2, int lineno)
{
	func_div(v1,v2,lineno);
	swap(registersTable[1]);
	jzero(20);
	swap(registersTable[1]);
	swap(registersTable[7]);
	jzero(11);
	swap(registersTable[6]);
	jzero(5);
	reset(registersTable[0]);
	add(registersTable[2]);
	sub(registersTable[1]);
	jump(11);
	reset(registersTable[0]);
	add(registersTable[1]);
	sub(registersTable[2]);
	jump(7);

	swap(registersTable[6]);
	jzero(4);
	reset(registersTable[0]);
	sub(registersTable[1]);
	jump(2);

	swap(registersTable[1]);
	swap(registersTable[5]);
	v1->type=EXPR;
}
void *condition_eq(var *v1,var *v2, int lineno)
{
	condition cond;
	cond.name="EQ";
	cond.pre_cond_index=command_list.size()-1;
	load_var(v1,v2,lineno);
	swap(registersTable[5]);
	reset(registersTable[0]);
	add(registersTable[6]);
	sub(registersTable[5]);
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_neq(var *v1,var *v2, int lineno)
{
	condition cond;
	cond.name="NEQ";
	cond.pre_cond_index=command_list.size()-1;
	load_var(v1,v2,lineno);
	swap(registersTable[5]);
	reset(registersTable[0]);
	add(registersTable[6]);
	sub(registersTable[5]);
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_le(var *v1,var *v2, int lineno)
{
	condition cond;
	cond.name="LE";
	cond.pre_cond_index=command_list.size()-1;
	load_var(v1,v2,lineno);
	swap(registersTable[5]);
	reset(registersTable[0]);
	add(registersTable[6]);
	sub(registersTable[5]);
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_ge(var *v1,var *v2, int lineno)
{
	condition cond;
	cond.name="GE";
	cond.pre_cond_index=command_list.size()-1;
	load_var(v1,v2,lineno);
	swap(registersTable[5]);
	reset(registersTable[0]);
	add(registersTable[6]);
	sub(registersTable[5]);
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_leq(var *v1,var *v2, int lineno)
{
	condition cond;
	cond.name="LEQ";
	cond.pre_cond_index=command_list.size()-1;
	load_var(v1,v2,lineno);
	swap(registersTable[5]);
	reset(registersTable[0]);
	add(registersTable[6]);
	sub(registersTable[5]);
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_geq(var *v1,var *v2, int lineno)
{
	condition cond;
	cond.name="GEQ";
	cond.pre_cond_index=command_list.size()-1;
	load_var(v1,v2,lineno);
	swap(registersTable[5]);
	reset(registersTable[0]);
	add(registersTable[6]);
	sub(registersTable[5]);
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}

void func_if()
{
	long long commandListSize=command_list.size()-1;
	condition if_start=condition_start[condition_start.size()-1];
	condition_start.pop_back();
	long long j=commandListSize-if_start.post_cond_index+1;
	if(if_start.name=="EQ")
	{
		command_list[if_start.post_cond_index]="JNEG " + std::to_string(j);
		command_list.insert(command_list.begin()+if_start.post_cond_index,"JPOS " + std::to_string(j+1));
	}
	else if(if_start.name=="NEQ")
	{
		command_list[if_start.post_cond_index]="JZERO " + std::to_string(j);
	}
	else if(if_start.name=="LE")
	{
		command_list[if_start.post_cond_index]="JZERO " + std::to_string(j);
		command_list.insert(command_list.begin()+if_start.post_cond_index,"JPOS " + std::to_string(j+1));
	}
	else if(if_start.name=="GE")
	{
		command_list[if_start.post_cond_index]="JZERO " + std::to_string(j);
		command_list.insert(command_list.begin()+if_start.post_cond_index,"JNEG " + std::to_string(j+1));
	}
	else if(if_start.name=="LEQ")
	{

		command_list[if_start.post_cond_index]="JPOS " + std::to_string(j);
	}
	else if(if_start.name=="GEQ")
	{
		command_list[if_start.post_cond_index]="JNEG " + std::to_string(j);
	}
}
void func_endif()
{
	long long commandListSize=command_list.size()-1;
	condition if_start=condition_start[condition_start.size()-1];
	condition_start.pop_back();
	long long j=commandListSize-if_start.post_cond_index+1;
	command_list.insert(command_list.begin()+if_start.post_cond_index+1, "JUMP " + std::to_string(j));
}
void func_else()
{
	long long commandListSize=command_list.size()-1;
	condition if_start=condition_start[condition_start.size()-1];
	condition_start.pop_back();
	condition cond;
	cond.name=if_start.name;
	cond.pre_cond_index=commandListSize;
	cond.post_cond_index=commandListSize;
	long long j=commandListSize-if_start.post_cond_index+2;
	if(if_start.name=="EQ")
	{
		command_list[if_start.post_cond_index]="JNEG " + std::to_string(j);
		command_list.insert(command_list.begin()+if_start.post_cond_index,"JPOS " + std::to_string(j+1));
		cond.post_cond_index=commandListSize+1;
	}
	else if(if_start.name=="NEQ")
	{
		command_list[if_start.post_cond_index]="JZERO " + std::to_string(j);
		cond.post_cond_index=commandListSize;
	}
	else if(if_start.name=="LE")
	{
		command_list[if_start.post_cond_index]="JZERO " + std::to_string(j);
		command_list.insert(command_list.begin()+if_start.post_cond_index,"JPOS " + std::to_string(j+1));
		cond.post_cond_index=commandListSize+1;
	}
	else if(if_start.name=="GE")
	{
		command_list[if_start.post_cond_index]="JZERO " + std::to_string(j);
		command_list.insert(command_list.begin()+if_start.post_cond_index,"JNEG " + std::to_string(j+1));
		cond.post_cond_index=commandListSize+1;
	}
	else if(if_start.name=="LEQ")
	{

		command_list[if_start.post_cond_index]="JPOS " + std::to_string(j);
		cond.post_cond_index=commandListSize;
	}
	else if(if_start.name=="GEQ")
	{
		command_list[if_start.post_cond_index]="JNEG " + std::to_string(j);
		cond.post_cond_index=commandListSize;
	}
	condition_start.push_back(cond);
}
void func_while()
{
	long long commandListSize=command_list.size()-1;
	condition if_start=condition_start[condition_start.size()-1];
	condition_start.pop_back();
	long long j=commandListSize-if_start.post_cond_index+2;

	long long revert=(commandListSize-if_start.pre_cond_index)*(-1);
	command_list.push_back("JUMP " + std::to_string(revert));
	if(if_start.name=="EQ")
	{
		command_list[if_start.post_cond_index]="JNEG " + std::to_string(j);
		command_list.insert(command_list.begin()+if_start.post_cond_index,"JPOS " + std::to_string(j+1));
	}
	else if(if_start.name=="NEQ")
	{
		command_list[if_start.post_cond_index]="JZERO " + std::to_string(j);
	}
	else if(if_start.name=="LE")
	{
		command_list[if_start.post_cond_index]="JZERO " + std::to_string(j);
		command_list.insert(command_list.begin()+if_start.post_cond_index,"JPOS " + std::to_string(j+1));
	}
	else if(if_start.name=="GE")
	{
		command_list[if_start.post_cond_index]="JZERO " + std::to_string(j);
		command_list.insert(command_list.begin()+if_start.post_cond_index,"JNEG " + std::to_string(j+1));
	}
	else if(if_start.name=="LEQ")
	{

		command_list[if_start.post_cond_index]="JPOS " + std::to_string(j);
	}
	else if(if_start.name=="GEQ")
	{
		command_list[if_start.post_cond_index]="JNEG " + std::to_string(j);
	}
}
void func_repeat()
{
	condition cond;
	cond.name="LEQ";
	cond.pre_cond_index=command_list.size()-1;
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void func_until()
{
	long long commandListSize=command_list.size()-1;
	condition repeat_ending=condition_start[condition_start.size()-1];
	condition_start.pop_back();
	condition repeat_beginning=condition_start[condition_start.size()-1];
	condition_start.pop_back();
	long long j=(commandListSize-repeat_beginning.post_cond_index-1)*(-1);
	if(repeat_ending.name=="EQ")
	{
		command_list[repeat_ending.post_cond_index]="JNEG " + std::to_string(j);
		command_list.insert(command_list.begin()+repeat_ending.post_cond_index,"JPOS " + std::to_string(j+1));
	}
	else if(repeat_ending.name=="NEQ")
	{
		command_list[repeat_ending.post_cond_index]="JZERO " + std::to_string(j);
	}
	else if(repeat_ending.name=="LE")
	{
		command_list[repeat_ending.post_cond_index]="JPOS " + std::to_string(j);
	}
	else if(repeat_ending.name=="GE")
	{
		command_list[repeat_ending.post_cond_index]="JNEG " + std::to_string(j);
	}
	else if(repeat_ending.name=="LEQ")
	{
		command_list[repeat_ending.post_cond_index]="JPOS " + std::to_string(j);
		command_list.insert(command_list.begin()+repeat_ending.post_cond_index,"JZERO " + std::to_string(j+1));
	}
	else if(repeat_ending.name=="GEQ")
	{
		command_list[repeat_ending.post_cond_index]="JNEG " + std::to_string(j);
		command_list.insert(command_list.begin()+repeat_ending.post_cond_index,"JZERO " + std::to_string(j+1));
	}
}


//ASSIGN for FOR temporary solution
void for_to(std::string name, var *v1, var *v2, int lineno)
{
	variable iter;
	iter.name=name;
	iter.value=v1->value;
	iter.type=_VAR;
	iterators.push_back(iter);
	init_iter(iter.name,lineno);
	init_iter(iter.name+"temp",lineno);
	var *current_var;
	current_var=new var;
	current_var->name=iter.name;
	current_var->value=(long long)0;
	current_var->type=_VAR;
	var *bound;
	bound=new var;
	bound->name=iter.name+"temp";
	bound->value=(long long)0;
	bound->type=_VAR;
	assign(current_var,v1,lineno);
	assign(bound,v2,lineno);
	condition_leq(current_var,bound,lineno);
	forLabel label;
	label.name=name;
	label.startValue=v1->value;
	label.startValue=v2->value;
	label.index=command_list.size()-1;
	forLabel_start.push_back(label);
}
void for_downto(std::string name, var *v1, var *v2, int lineno)
{
	variable iter;
	iter.name=name;
	iter.value=v1->value;
	iter.type=_VAR;
	iterators.push_back(iter);
	init_iter(iter.name,lineno);
	init_iter(iter.name+"temp",lineno);
	var *current_var;
	current_var=new var;
	current_var->name=iter.name;
	current_var->value=(long long)0;
	current_var->type=_VAR;
	var *bound;
	bound=new var;
	bound->name=iter.name+"temp";
	bound->value=(long long)0;
	bound->type=_VAR;
	assign(current_var,v1,lineno);
	assign(bound,v2,lineno);
	condition_geq(current_var,bound,lineno);
	forLabel label;
	label.name=name;
	label.startValue=v1->value;
	label.startValue=v2->value;
	label.index=command_list.size()-1;
	forLabel_start.push_back(label);
}
void endfor_to()
{
	variable iter=iterators[iterators.size()-1];
	iterators.pop_back();
	int i=getsym(iter.name);
	reset(registersTable[0]);
	generateConst(sym_tab[i].position);
	swap(registersTable[1]);
	load(registersTable[1]);
	inc(registersTable[0]);
	store(registersTable[1]);
	sym_tab.pop_back();
	sym_tab.pop_back();
	long long commandListSize=command_list.size()-1;
	forLabel for_start=forLabel_start[forLabel_start.size()-1];
	forLabel_start.pop_back();
	condition if_start=condition_start[condition_start.size()-1];
	condition_start.pop_back();
	long long j=commandListSize-if_start.pre_cond_index;
	long long pre=if_start.post_cond_index;
	command_list[pre]="JPOS " + std::to_string(commandListSize-if_start.post_cond_index+2);
	command_list.push_back("JUMP "+ std::to_string(j*(-1)));



}
void endfor_downto()
{
	variable iter=iterators[iterators.size()-1];
	iterators.pop_back();
	int i=getsym(iter.name);
	reset(registersTable[0]);
	generateConst(sym_tab[i].position);
	swap(registersTable[1]);
	load(registersTable[1]);
	dec(registersTable[0]);
	store(registersTable[1]);
	sym_tab.pop_back();
	sym_tab.pop_back();
	long long commandListSize=command_list.size()-1;
	forLabel for_start=forLabel_start[forLabel_start.size()-1];
	forLabel_start.pop_back();
	condition if_start=condition_start[condition_start.size()-1];
	condition_start.pop_back();
	long long j=commandListSize-if_start.pre_cond_index;
	long long pre=if_start.post_cond_index;
	command_list[pre]="JNEG " + std::to_string(commandListSize-if_start.post_cond_index+2);
	command_list.push_back("JUMP "+ std::to_string(j*(-1)));
}
/*----------------------------*/
