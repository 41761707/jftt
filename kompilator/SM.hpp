#pragma once
#include "ST.hpp"
#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>

int current_command=0;
std::vector<std::string> command_list;
struct var_init
{
    std::string name;
    long long int lineno;
};
std::vector<var_init> not_init;

struct memoryCells
{
	std::string name;
	long long value;
};

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
    PTR
};

struct variable
{
    std::string name;
    long long value;
    var_type type;
};
typedef struct variable var;


struct condition
{
    var *sourceA;
    var *sourceB;
    long long value;
};
typedef struct condition condition;

struct local_var
{
    std::string name;
    long long int index;
};
typedef struct local_var lVar;

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
	for(int i=0;i<sym_tab.size();i++)
	{
		if(sym_tab[i].type==VARIABLE)
		{
			memoryCells cell;
			cell.name=sym_tab[i].name;
			cell.value=0;
			memory.push_back(cell);
		}
		else
		{
			int j=0;
			while (j<sym_tab[i].length)
			{
				memoryCells cell;
				int index=sym_tab[i].firstIndex+j;
				cell.name=sym_tab[i].name+std::to_string(index);
				cell.value=0;
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
var *func_num(long long int value, int lineno)
{
    var *current_var;
    current_var = new var;
    current_var->value = value;
    current_var->type = VAL;
    return current_var;
}

var *func_id(var *variable,int lineno)
{
	var *current_var;
    current_var = new var;
    current_var->name = variable->name;
    current_var->type = _VAR;
    return current_var;
}

var *func_pid(std::string name, int lineno)
{
	var *current_var;
	current_var = new var;
	current_var->name = name;
	current_var->value = (long long )1;
	current_var->type = _VAR;
	return current_var;
}

var *func_pid_arr(std::string name, std::string index, int lieno)
{

}

var *func_pid(std::string name, long long index, int lineno)
{
	std::string arrayIndex=name+std::to_string(index);
	for(int i=0;i<memory.size();i++)
	{
		if(memory[i].name==arrayIndex)
		{
			var *current_var;
			current_var=new var;
			current_var->name=arrayIndex;
			current_var->value=(long long)1;
			current_var->type = _VAR;
			return current_var;
		}

	}
}

var *func_val(var *val, int lineno)
{

}

/* COMMANDS */
/*----------------------------*/
void halt()
{
	command_list.push_back("HALT");
	for(int i=0;i<command_list.size();i++)
	{
		std::cout << command_list[i] << std::endl;
	}
	test_print();
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
	reg.value=0;
	current_command=current_command+1;
}
void inc(registers &reg)
{
	reg.value=reg.value+1;
	command_list.push_back("INC " + reg.registerName);
	current_command=current_command+1;
}
void dec(registers &reg)
{
	reg.value=reg.value-1;
	command_list.push_back("DEC " + reg.registerName);
	current_command=current_command+1;
}
void get(std::string name)
{
	command_list.push_back("GET");
	registersTable[0].variableName=name;
	std::cout << "INPUT: " << std::endl;
	std::cin >> registersTable[0].value;
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
	reg.variableName=memory[i].name;
	reg.value=memory[i].value;
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
	memory[i].value=registersTable[0].value;
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
	bool negative=false;
	if(current->value<0)
	{
		negative=true;
	}
	reset(registersTable[0]);
	if(current->type==_VAR)
	{
		reset(registersTable[1]);
		load(current->name,registersTable[1]);
		put();
	}
	else if(current->type==VAL)
	{
		if(negative)
		{
			int a=current->value;
			for(int i=current->value;i<0;i++)
			{
				dec(registersTable[0]);
			}
		}
		else
		{
			for(int i=0;i<current->value;i++)
			{
				inc(registersTable[0]);
			}
		}
		put();
	}
}
void assign(var *variable, var *expr, int lineno)
{
	if(expr->type==_VAR)
	{
		std::cout << "EXPR->NAME" << std::endl;
		std::cout << expr->name << std::endl;
		// 1 = 2
		reset(registersTable[0]);
		reset(registersTable[1]);
		reset(registersTable[2]);
		load(expr->name,registersTable[0]);  // 2
		swap(registersTable[2]);
		reset(registersTable[1]);
		load(variable->name,registersTable[1]); // 1
		reset_value_only(registersTable[0]);
		add(registersTable[2]);
		reset(registersTable[1]);
		store(variable->name,registersTable[1]);

	}
	else if(expr->type==VAL)
	{
		reset(registersTable[0]);
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
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==variable->name)
			{
				load(variable->name,registersTable[2]);
				registersTable[0].value=0;
				break;
			}
		}
		reset_value_only(registersTable[0]);
		add(registersTable[1]);
		reset(registersTable[2]);
		store(variable->name,registersTable[2]);

	}
}
var *func_plus( var *v1, var *v2, int lineno)
{
	//liczba- 0, zmienna- 1
	if(v1->type==VAL && v2->type==VAL)
	{
		v1->value=v1->value+v2->value;
		return v1;
	}
	else if(v1->type==VAL && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else if(v1->type==_VAR && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
	}
	var *current_var;
    current_var = new var;
    current_var->name="PLUS RESULT";
    current_var->value=v1->value+v2->value;
    current_var->type = VAL;
    return current_var;

}
var *func_minus( var *v1, var *v2, int lineno)
{
	//liczba- 0, zmienna- 1
	if(v1->type==VAL && v2->type==VAL)
	{
		v1->value=v1->value-v2->value;
		return v1;
	}
	else if(v1->type==VAL && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else if(v1->type==_VAR && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
	}
	var *current_var;
    current_var = new var;
    current_var->name="PLUS RESULT";
    current_var->value=v1->value-v2->value;
    current_var->type = VAL;
    return current_var;

}
var *func_times( var *v1, var *v2, int lineno)
{
	//liczba- 0, zmienna- 1
	if(v1->type==VAL && v2->type==VAL)
	{
		v1->value=v1->value*v2->value;
		return v1;
	}
	else if(v1->type==VAL && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else if(v1->type==_VAR && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
	}
	var *current_var;
    current_var = new var;
    current_var->name="PLUS RESULT";
    current_var->value=v1->value*v2->value;
    current_var->type = VAL;
    return current_var;

}
var *func_div( var *v1, var *v2, int lineno)
{
	//liczba- 0, zmienna- 1
	if(v1->type==VAL && v2->type==VAL)
	{
		v1->value=v1->value/v2->value;
		return v1;
	}
	else if(v1->type==VAL && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else if(v1->type==_VAR && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
	}
	var *current_var;
    current_var = new var;
    current_var->name="PLUS RESULT";
    current_var->value=v1->value/v2->value;
    current_var->type = VAL;
    return current_var;

}
var *func_mod( var *v1, var *v2, int lineno)
{
	//liczba- 0, zmienna- 1
	if(v1->type==VAL && v2->type==VAL)
	{
		v1->value=v1->value%v2->value;
		return v1;
	}
	else if(v1->type==VAL && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else if(v1->type==_VAR && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				v2->value=memory[i].value;
			}
		}
	}
	else
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				v1->value=memory[i].value;
			}
		}
	}
	var *current_var;
    current_var = new var;
    current_var->name="PLUS RESULT";
    current_var->value=v1->value%v2->value;
    current_var->type = VAL;
    return current_var;

}
var *condition_equal(var *a, var *b, int lineno)
{
	condition *cond;
	cond = new condition;
	cond->sourceA=a;
	cond->sourceB=b;
	cond->value=a->value-b->value;
	reset(registersTable[0]);	
	return cond;

}
/*----------------------------*/