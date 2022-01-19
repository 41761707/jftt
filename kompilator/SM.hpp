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

int current_command=0;
int codeErrors=0;
std::vector<std::string> command_list;
/*struct var_init
{
    std::string name;
    long long int lineno;
};
std::vector<var_init> not_init;*/


struct registers
{
	std::string registerName;
	std::string variableName;
	long long value;
};

enum var_type
{
    VAL,
    _VAR,
    ARR,
    ARRIND,
    EXPR
};

struct memoryCells
{
	std::string name;
	long long value;
	var_type type;	
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

/*struct local_var
{
    std::string name;
    long long int index;
};
typedef struct local_var lVar;*/

typedef struct memoryCells memoryCells;
typedef struct registers registers;
registers registersTable[8];
std::vector<memoryCells> memory;


void init_registers()
{
	registersTable[0].registerName="a";
	registersTable[0].variableName="";
	registersTable[1].registerName="b";
	registersTable[1].variableName="";
	registersTable[2].registerName="c";
	registersTable[2].variableName="";
	registersTable[3].registerName="d";
	registersTable[3].variableName="";
	registersTable[4].registerName="e";
	registersTable[4].variableName="";
	registersTable[5].registerName="f";
	registersTable[5].variableName="";
	registersTable[6].registerName="g";
	registersTable[6].variableName="";
	registersTable[7].registerName="h";
	registersTable[7].variableName="";
	condition cond;
	condition_start.push_back(cond);
	forLabel forLabel;
	forLabel_start.push_back(forLabel);
	for(int i=0;i<sym_tab.size();i++)
	{
		if(sym_tab[i].type==VARIABLE)
		{
			memoryCells cell;
			cell.name=sym_tab[i].name;
			cell.value=0;
			cell.type=_VAR;
			memory.push_back(cell);
		}
		else
		{
			memoryCells arrayName;
			arrayName.name=sym_tab[i].name;
			arrayName.value=0;
			arrayName.type=ARR;
			memory.push_back(arrayName);
			int j=0;
			while (j<sym_tab[i].length)
			{
				memoryCells cell;
				int index=sym_tab[i].firstIndex+j;
				cell.name=sym_tab[i].name+std::to_string(index);
				cell.value=index;
				cell.type=ARR;
				memory.push_back(cell);
				j=j+1;
			}
		}
	}
}

void test_print()
{
	for(int i=0;i<8;i++)
	{
		std::cout << registersTable[i].registerName << ", " << registersTable[i].variableName << ", " << registersTable[i].value << std::endl;
	}
	for(int i=0;i<sym_tab.size();i++)
	{
		std::cout << i << ", " << sym_tab[i].name << std::endl;
	}
	for(int i=0;i<memory.size();i++)
	{
		std::cout << i << ", " << memory[i].name <<", " << memory[i].value << std::endl;
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
	/*for(int i=0;i<command_list.size();i++)
	{
		std::cout << command_list[i] << std::endl;
	}*/
	//test_print();
}
void reset(registers &reg)
{
	command_list.push_back("RESET " + reg.registerName);
	reg.value=0;
	reg.variableName="";
	current_command=current_command+1;
}
void reset_value_only(registers &reg)
{
	command_list.push_back("RESET " + reg.registerName);
	current_command=current_command+1;
}
void inc(registers &reg)
{
	command_list.push_back("INC " + reg.registerName);
	current_command=current_command+1;
}
void dec(registers &reg)
{
	command_list.push_back("DEC " + reg.registerName);
	current_command=current_command+1;
}
void get(std::string name)
{
	command_list.push_back("GET");
	registersTable[0].variableName=name;
	current_command=current_command+1;
}
void add(registers &reg)
{
	registersTable[0].value=registersTable[0].value+reg.value;
	command_list.push_back("ADD " + reg.registerName);
	current_command=current_command+1;
}
void sub(registers &reg)
{
	registersTable[0].value=registersTable[0].value-reg.value;
	command_list.push_back("SUB " + reg.registerName);
	current_command=current_command+1;
}
void swap(registers &reg)
{
	command_list.push_back("SWAP " + reg.registerName);
	std::string tempName=reg.variableName;
	long long tempValue=reg.value;
	reg.variableName=registersTable[0].variableName;
	reg.value=registersTable[0].value;

	registersTable[0].variableName=tempName;
	registersTable[0].value=tempValue;
	current_command=current_command+1;
}
void put()
{
	command_list.push_back("PUT");
	current_command=current_command+1;
}
void load(std::string name, registers &reg)
{
	int i;
	for(i=0;i<memory.size();i++)
	{
		if(memory[i].name==name)
		{
			break;
		}
	}
	int a;
	for(a=0;a<i;a++)
	{
		inc(reg);
	}
	registersTable[0].variableName=memory[i].name;
	command_list.push_back("LOAD " + reg.registerName);
	current_command=current_command+1;
}
void load_without_name(registers &reg)
{
	command_list.push_back("LOAD " + reg.registerName);
	current_command=current_command+1;
}
void store(std::string name,registers &reg)
{
	int i;
	for(i=0;i<memory.size();i++)
	{
		if(memory[i].name==name)
		{
			break;
		}
	}
	reg.variableName=name;
	int a;
	for(a=0;a<i;a++)
	{
		inc(reg);
	}
	command_list.push_back("STORE " + reg.registerName);
	current_command=current_command+1;

}
void store_bez_inc(registers &reg)
{
	command_list.push_back("STORE " + reg.registerName);
	current_command=current_command+1;
}
void shift(registers &reg)
{
	registersTable[0].value=floor(pow(2,reg.value)*registersTable[0].value);
	command_list.push_back("SHIFT " + reg.registerName);
	current_command=current_command+1;
}
void jump(long long j)
{
	command_list.push_back("JUMP " + std::to_string(j));
	current_command=current_command+j;
}
void jpos(long long j)
{
	if(registersTable[0].value>0)
	{
		current_command=current_command+j;
	}
	command_list.push_back("JPOS " + std::to_string(j));
	current_command=current_command+1;

}
void jzero(long long j)
{
	if(registersTable[0].value==0)
	{
		current_command=current_command+j;
	}
	command_list.push_back("JZERO " + std::to_string(j));
	current_command=current_command+1;

}
void jneg(long long j)
{
	if(registersTable[0].value<0)
	{
		current_command=current_command+j;
	}
	command_list.push_back("JNEG " + std::to_string(j));
	current_command=current_command+1;

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
	current_var->index=(long long)-1;
	current_var->type=ARR;  //TU UWAGA
	current_var->value=index;
	return current_var;

}

/*GRAMMAR FUNCTIONS */
/*----------------------------*/
void read(var *name, int lineno)
{
	reset(registersTable[0]);
	reset(registersTable[1]);
	load(name->name,registersTable[1]);
	get(name->name);
	reset(registersTable[2]);
	store(name->name,registersTable[2]);

}
void write(var *current, int lineno)
{
	reset(registersTable[0]);
	if(current->type==VAL)
	{
		if(current->value<0)
		{
			int a=current->value;
			std::string number=decToBin(current->value*(-1));
			//std::cout << number << std::endl;
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
				//std::cout << "SIEMA" << std::endl;
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
			int a=current->value;
			std::string number=decToBin(current->value);
			std::cout << number << std::endl;
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
				//std::cout << "SIEMA" << std::endl;
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
		put();
	}
	else if(current->type==_VAR)
	{
		reset(registersTable[1]);
		load(current->name,registersTable[1]);
		put();
	}
	else if(current->type==ARR)
	{
		std::string newString=current->name+std::to_string(current->value);
		//std::cout << newString << std::endl;
		reset(registersTable[1]);
		load(newString,registersTable[1]);
		put();

	}
	else if(current->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[1]);
		load(current->index,registersTable[1]);
		swap(registersTable[1]);
		reset(registersTable[2]);
		load(current->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		put();
	}
}
void assign(var *variable, var *expr, int lineno)
{
	std::cout << variable->type << std::endl;
	//TODO: x ASSIGN tab[a] EXPR tab[a]
	if(variable->type==_VAR)
	{
		reset(registersTable[2]);
		load(variable->name,registersTable[2]);
		reset(registersTable[0]);
		if(expr->type==EXPR)
		{
		}
		else if(expr->type==VAL)
		{
			reset(registersTable[1]);
			if(expr->value>0)
			{
				for(int i=0;i<expr->value;i++)
				{
					inc(registersTable[1]);
				}
			}
			else
			{
				for(int i=expr->value;i<0;i++)
				{
					dec(registersTable[1]);
				}
			}
		}
		else
		{
			reset(registersTable[1]);
			load(expr->name,registersTable[1]);
			swap(registersTable[1]);
		}
		reset(registersTable[0]);
		add(registersTable[1]);
		store_bez_inc(registersTable[2]);
	}
	else if(variable->type==ARR)
	{
		std::string newString=variable->name+std::to_string(variable->value);
		reset_value_only(registersTable[0]);
		if(expr->type==EXPR)
		{
		}
		else if(expr->type==VAL)
		{
			reset(registersTable[1]);
			if(expr->value>0)
			{
				for(int i=0;i<expr->value;i++)
				{
					inc(registersTable[1]);
				}
			}
			else
			{
				for(int i=expr->value;i<0;i++)
				{
					dec(registersTable[1]);
				}
			}
		}
		else
		{
			reset(registersTable[1]);
			load(expr->name,registersTable[1]);
			swap(registersTable[1]);
		}
		reset(registersTable[0]);
		add(registersTable[1]);
		reset(registersTable[2]);
		store(newString,registersTable[2]);
	}
	else if(variable->type==ARRIND)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==variable->name)
			{
				std::cout << memory[i+1].value << std::endl;
			}
		}
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(variable->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(variable->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);
		reset(registersTable[0]);
		if(expr->type==EXPR)
		{
		}
		else if(expr->type==VAL)
		{
			reset(registersTable[1]);
			if(expr->value>0)
			{
				for(int i=0;i<expr->value;i++)
				{
					inc(registersTable[1]);
				}
			}
			else
			{
				for(int i=expr->value;i<0;i++)
				{
					dec(registersTable[1]);
				}
			}
		}
		else
		{
			reset(registersTable[1]);
			load(expr->name,registersTable[1]);
			swap(registersTable[1]);
		}
		add(registersTable[1]);
		store_bez_inc(registersTable[3]);	
	}
}
void *func_plus( var *v1, var *v2, int lineno)
{
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(v1->type==_VAR)
	{
		load(v1->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type==VAL)
	{
		if(v1->value>0)
		{
			for(int i=0;i<v1->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=v1->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(v1->type==ARRIND)
	{
		load(v1->index,registersTable[1]);
		swap(registersTable[1]);
		load(v1->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type=ARR)
	{
		std::string newString=v1->name+std::to_string(v1->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(v2->type==_VAR)
	{
		load(v2->name,registersTable[2]);
	}
	else if(v2->type==VAL)
	{
		if(v2->value>0)
		{
			for(int i=0;i<v2->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=v2->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(v2->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(v2->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(v2->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(v2->type=ARR)
	{
		std::string newString=v2->name+std::to_string(v2->value);
		load(newString,registersTable[2]);
		
	}
	add(registersTable[1]);
	swap(registersTable[1]);
	v1->type=EXPR;
}
void *func_minus( var *v1, var *v2, int lineno)
{
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(v1->type==_VAR)
	{
		load(v1->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type==VAL)
	{
		if(v1->value>0)
		{
			for(int i=0;i<v1->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=v1->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(v1->type==ARRIND)
	{
		load(v1->index,registersTable[1]);
		swap(registersTable[1]);
		load(v1->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type=ARR)
	{
		std::string newString=v1->name+std::to_string(v1->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(v2->type==_VAR)
	{
		load(v2->name,registersTable[2]);
	}
	else if(v2->type==VAL)
	{
		if(v2->value>0)
		{
			for(int i=0;i<v2->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=v2->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(v2->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(v2->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(v2->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(v2->type=ARR)
	{
		std::string newString=v2->name+std::to_string(v2->value);
		load(newString,registersTable[2]);
		
	}
	swap(registersTable[1]);
	sub(registersTable[1]);
	swap(registersTable[1]);
	v1->type=EXPR;

}
void *func_times( var *v1, var *v2, int lineno)
{

	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	reset(registersTable[3]);
	reset(registersTable[4]);
	if(v1->type==_VAR)
	{
		load(v1->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type==VAL)
	{
		if(v1->value>0)
		{
			for(int i=0;i<v1->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=v1->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(v1->type==ARRIND)
	{
		load(v1->index,registersTable[1]);
		swap(registersTable[1]);
		load(v1->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type=ARR)
	{
		std::string newString=v1->name+std::to_string(v1->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(v2->type==_VAR)
	{
		load(v2->name,registersTable[2]);
	}
	else if(v2->type==VAL)
	{
		if(v2->value>0)
		{
			for(int i=0;i<v2->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=v2->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(v2->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(v2->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(v2->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(v2->type=ARR)
	{
		std::string newString=v2->name+std::to_string(v2->value);
		load(newString,registersTable[2]);
		
	}
	jzero(38);
	swap(registersTable[2]);
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
	swap(registersTable[1]);
	jump(3);

	reset(registersTable[0]);
	swap(registersTable[1]);
	/*reset(registersTable[2]);
	swap(registersTable[2]);
	swap(registersTable[1]);
	jpos(9);
	swap(registersTable[1]);
	sub(registersTable[1]);
	swap(registersTable[2]);
	swap(registersTable[1]);
	reset(registersTable[0]);
	sub(registersTable[1]);
	swap(registersTable[2]);
	reset(registersTable[1]);
	jzero(6);
	dec(registersTable[0]);
	swap(registersTable[1]);
	add(registersTable[2]);
	swap(registersTable[1]);
	jump(-5);
	swap(registersTable[1]);
	reset(registersTable[1]);
	swap(registersTable[1]);*/

	v1->type=EXPR;

}
void *func_div(var *v1, var *v2, int lineno)
{
	//a->v1, b->v2, c->0,
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(v1->type==_VAR)
	{
		load(v1->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type==VAL)
	{
		if(v1->value>0)
		{
			for(int i=0;i<v1->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=v1->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(v1->type==ARRIND)
	{
		load(v1->index,registersTable[1]);
		swap(registersTable[1]);
		load(v1->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type=ARR)
	{
		std::string newString=v1->name+std::to_string(v1->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(v2->type==_VAR)
	{
		load(v2->name,registersTable[2]);
	}
	else if(v2->type==VAL)
	{
		if(v2->value>0)
		{
			for(int i=0;i<v2->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=v2->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(v2->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(v2->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(v2->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(v2->type=ARR)
	{
		std::string newString=v2->name+std::to_string(v2->value);
		load(newString,registersTable[2]);
		
	}
	jzero(45);
	swap(registersTable[2]);
	swap(registersTable[1]);
	jzero(42);
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
	swap(registersTable[1]);
	jump(3);

	reset(registersTable[0]);
	swap(registersTable[1]);
	v1->type=EXPR;


}
void *func_mod(var *v1, var *v2, int lineno)
{
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(v1->type==_VAR)
	{
		load(v1->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type==VAL)
	{
		if(v1->value>0)
		{
			for(int i=0;i<v1->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=v1->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(v1->type==ARRIND)
	{
		load(v1->index,registersTable[1]);
		swap(registersTable[1]);
		load(v1->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(v1->type=ARR)
	{
		std::string newString=v1->name+std::to_string(v1->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(v2->type==_VAR)
	{
		load(v2->name,registersTable[2]);
	}
	else if(v2->type==VAL)
	{
		if(v2->value>0)
		{
			for(int i=0;i<v2->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=v2->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(v2->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(v2->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(v2->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(v2->type=ARR)
	{
		std::string newString=v2->name+std::to_string(v2->value);
		load(newString,registersTable[2]);
		
	}
	jzero(41);
	swap(registersTable[2]);
	swap(registersTable[1]);
	jzero(38);
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

	v1->type=EXPR;
}
void *condition_eq(var *a,var *b, int lineno)
{
	condition cond;
	cond.name="EQ";
	cond.pre_cond_index=command_list.size()-1;
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(a->type==_VAR)
	{
		load(a->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type==VAL)
	{
		if(a->value>0)
		{
			for(int i=0;i<a->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=a->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(a->type==ARRIND)
	{
		load(a->index,registersTable[1]);
		swap(registersTable[1]);
		load(a->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type=ARR)
	{
		std::string newString=a->name+std::to_string(a->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(b->type==_VAR)
	{
		load(b->name,registersTable[2]);
	}
	else if(b->type==VAL)
	{
		if(b->value>0)
		{
			for(int i=0;i<b->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=b->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(b->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(b->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(b->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(b->type=ARR)
	{
		std::string newString=b->name+std::to_string(b->value);
		load(newString,registersTable[2]);
		
	}
	swap(registersTable[2]);
	reset(registersTable[0]);
	add(registersTable[1]);
	sub(registersTable[2]);
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_neq(var *a,var *b, int lineno)
{
	condition cond;
	cond.name="NEQ";
	cond.pre_cond_index=command_list.size()-1;
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(a->type==_VAR)
	{
		load(a->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type==VAL)
	{
		if(a->value>0)
		{
			for(int i=0;i<a->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=a->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(a->type==ARRIND)
	{
		load(a->index,registersTable[1]);
		swap(registersTable[1]);
		load(a->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type=ARR)
	{
		std::string newString=a->name+std::to_string(a->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(b->type==_VAR)
	{
		load(b->name,registersTable[2]);
	}
	else if(b->type==VAL)
	{
		if(b->value>0)
		{
			for(int i=0;i<b->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=b->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(b->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(b->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(b->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(b->type=ARR)
	{
		std::string newString=b->name+std::to_string(b->value);
		load(newString,registersTable[2]);
		
	}
	swap(registersTable[2]);
	reset(registersTable[0]);
	add(registersTable[1]);
	sub(registersTable[2]);	
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_le(var *a,var *b, int lineno)
{
	condition cond;
	cond.name="LE";
	cond.pre_cond_index=command_list.size()-1;
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(a->type==_VAR)
	{
		load(a->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type==VAL)
	{
		if(a->value>0)
		{
			for(int i=0;i<a->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=a->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(a->type==ARRIND)
	{
		load(a->index,registersTable[1]);
		swap(registersTable[1]);
		load(a->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type=ARR)
	{
		std::string newString=a->name+std::to_string(a->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(b->type==_VAR)
	{
		load(b->name,registersTable[2]);
	}
	else if(b->type==VAL)
	{
		if(b->value>0)
		{
			for(int i=0;i<b->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=b->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(b->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(b->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(b->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(b->type=ARR)
	{
		std::string newString=b->name+std::to_string(b->value);
		load(newString,registersTable[2]);
		
	}
	swap(registersTable[2]);
	reset(registersTable[0]);
	add(registersTable[1]);
	sub(registersTable[2]);
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_ge(var *a,var *b, int lineno)
{
	condition cond;
	cond.name="GE";
	cond.pre_cond_index=command_list.size()-1;
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(a->type==_VAR)
	{
		load(a->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type==VAL)
	{
		if(a->value>0)
		{
			for(int i=0;i<a->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=a->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(a->type==ARRIND)
	{
		load(a->index,registersTable[1]);
		swap(registersTable[1]);
		load(a->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type=ARR)
	{
		std::string newString=a->name+std::to_string(a->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(b->type==_VAR)
	{
		load(b->name,registersTable[2]);
	}
	else if(b->type==VAL)
	{
		if(b->value>0)
		{
			for(int i=0;i<b->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=b->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(b->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(b->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(b->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(b->type=ARR)
	{
		std::string newString=b->name+std::to_string(b->value);
		load(newString,registersTable[2]);
		
	}
	swap(registersTable[2]);
	reset(registersTable[0]);
	add(registersTable[1]);
	sub(registersTable[2]);	
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_leq(var *a,var *b, int lineno)
{
	condition cond;
	cond.name="LEQ";
	cond.pre_cond_index=command_list.size()-1;
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(a->type==_VAR)
	{
		load(a->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type==VAL)
	{
		if(a->value>0)
		{
			for(int i=0;i<a->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=a->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(a->type==ARRIND)
	{
		load(a->index,registersTable[1]);
		swap(registersTable[1]);
		load(a->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type=ARR)
	{
		std::string newString=a->name+std::to_string(a->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(b->type==_VAR)
	{
		load(b->name,registersTable[2]);
	}
	else if(b->type==VAL)
	{
		if(b->value>0)
		{
			for(int i=0;i<b->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=b->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(b->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(b->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(b->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(b->type=ARR)
	{
		std::string newString=b->name+std::to_string(b->value);
		load(newString,registersTable[2]);
		
	}
	swap(registersTable[2]);
	reset(registersTable[0]);
	add(registersTable[1]);
	sub(registersTable[2]);
	command_list.push_back("PLACEHOLDER");
	cond.post_cond_index=command_list.size()-1;
	condition_start.push_back(cond);
}
void *condition_geq(var *a,var *b, int lineno)
{
	condition cond;
	cond.name="GEQ";
	cond.pre_cond_index=command_list.size()-1;
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	if(a->type==_VAR)
	{
		load(a->name,registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type==VAL)
	{
		if(a->value>0)
		{
			for(int i=0;i<a->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=a->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
	}
	else if(a->type==ARRIND)
	{
		load(a->index,registersTable[1]);
		swap(registersTable[1]);
		load(a->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		load_without_name(registersTable[1]);
		swap(registersTable[1]);
	}
	else if(a->type=ARR)
	{
		std::string newString=a->name+std::to_string(a->value);
		load(newString,registersTable[1]);
		swap(registersTable[1]);
	}
	reset(registersTable[0]);
	if(b->type==_VAR)
	{
		load(b->name,registersTable[2]);
	}
	else if(b->type==VAL)
	{
		if(b->value>0)
		{
			for(int i=0;i<b->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		else
		{
			for(int i=b->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
	}
	else if(b->type==ARRIND)
	{
		reset(registersTable[0]);
		reset(registersTable[3]);
		load(b->index,registersTable[3]);
		swap(registersTable[3]);
		reset(registersTable[2]);
		load(b->name+"0",registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[3]);
		add(registersTable[2]);
		swap(registersTable[3]);
		reset(registersTable[0]);
		load_without_name(registersTable[3]);

	}
	else if(b->type=ARR)
	{
		std::string newString=b->name+std::to_string(b->value);
		load(newString,registersTable[2]);
		
	}
	swap(registersTable[2]);
	add(registersTable[1]);
	sub(registersTable[2]);
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
	memoryCells cell;
	cell.name=name;
	cell.value=v1->value;
	cell.type=_VAR;
	memory.push_back(cell);
	variable iter;
	iter.name=name;
	iter.value=v1->value;
	iter.type=_VAR;
	iterators.push_back(iter);
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	load(iter.name,registersTable[2]);
	load(v1->name,registersTable[1]);
	store_bez_inc(registersTable[2]);
	condition_leq(&iter,v2,lineno);
	forLabel label;
	label.name=name;
	label.startValue=v1->value;
	label.startValue=v2->value;
	label.index=command_list.size()-1;
	forLabel_start.push_back(label);
}
void for_downto(std::string name, var *v1, var *v2, int lineno)
{
	memoryCells cell;
	cell.name=name;
	cell.value=v1->value;
	cell.type=_VAR;
	memory.push_back(cell);
	variable iter;
	iter.name=name;
	iter.value=v1->value;
	iter.type=_VAR;
	iterators.push_back(iter);
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	load(iter.name,registersTable[2]);
	load(v1->name,registersTable[1]);
	store_bez_inc(registersTable[2]);
	condition_geq(&iter,v2,lineno);
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
	reset(registersTable[1]);
	load(iter.name,registersTable[1]);
	inc(registersTable[0]);
	store_bez_inc(registersTable[1]);
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
	reset(registersTable[1]);
	load(iter.name,registersTable[1]);
	dec(registersTable[0]);
	store_bez_inc(registersTable[1]);
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
