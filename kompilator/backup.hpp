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
std::vector<std::string> command_list;
/*struct var_init
{
    std::string name;
    long long int lineno;
};
std::vector<var_init> not_init;*/

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
    PTR,
};

struct variable
{
    std::string name;
    std::string loopName;
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
	//std::cout << "INPUT: " << std::endl;
	//std::cin >> registersTable[0].value;
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
	registersTable[0].value=memory[i].value;
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
void store_bez_inc(std::string name, registers &reg)
{
	reg.variableName=name;
	memory[reg.value].value=registersTable[0].value;
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
	std::cout << "func_pid"<< std::endl;
	var *current_var;
	current_var = new var;
	current_var->name = name;
	current_var->value = (long long )1;
	current_var->type = _VAR;
	return current_var;
}

var *func_pid_arr(std::string name, std::string index, int lieno)
{
	//wczytaj index i pierwszy element tablicy, dodaj i zamontuj w pamięci
	std::cout << "func_pid_arr" << std::endl;
	reset(registersTable[0]);
	reset(registersTable[1]);
	load(name+"0",registersTable[1]);
	reset(registersTable[0]);
	reset(registersTable[2]);
	load(index,registersTable[2]);
	swap(registersTable[2]);
	reset(registersTable[0]);
	add(registersTable[1]);
	add(registersTable[2]);
	swap(registersTable[1]);
	test_print();
	var *current_var;
	current_var=new var;
	current_var->name=name+std::to_string(registersTable[2].value);
	current_var->value=(long long)1;
	current_var->type = PTR;
	return current_var;

}

var *func_pid(std::string name, long long index, int lineno)
{
	std::cout << "func_pid" << std::endl;
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
	if(current->type==_VAR)
	{
		swap(registersTable[1]);
		put();
		//reset(registersTable[1]);
		//load(current->name,registersTable[1]);
		//put();
	}
	else if(current->type==PTR)
	{
		std::cout << "SIEMA" << std::endl;
	}
	else if(current->type==VAL)
	{
		if(current->value<0)
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
	if(expr->type==_VAR && expr->name=="temp")
	{

		//std::cout << expr->name << std::endl;
		// 1 = 2
		reset(registersTable[1]);
		swap(registersTable[1]);
		reset(registersTable[2]);
		load(variable->name,registersTable[2]);
		reset_value_only(registersTable[0]);
		add(registersTable[1]);
		reset(registersTable[2]);
		store(variable->name,registersTable[2]);
		/*reset(registersTable[0]);
		reset(registersTable[1]);
		reset(registersTable[2]);
		load(expr->name,registersTable[0]);  // 2
		swap(registersTable[2]);
		reset(registersTable[1]);
		load(variable->name,registersTable[1]); // 1
		reset_value_only(registersTable[0]);
		add(registersTable[2]);
		reset(registersTable[1]);
		store(variable->name,registersTable[1]);*/

	}
	else if(variable->type==PTR && expr->type!=PTR)
	{
		std::string concat=variable->name+std::to_string(variable->value);
		reset(registersTable[0]);
		reset(registersTable[1]);
		reset(registersTable[2]);
		load(variable->name+"0",registersTable[1]);
		reset(registersTable[0]);
		load(variable->loopName,registersTable[2]);
		swap(registersTable[2]);
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		swap(registersTable[1]);
		reset(registersTable[0]);
		reset(registersTable[2]);
		load(expr->name,registersTable[2]);
		swap(registersTable[2]);
		reset(registersTable[3]);
		load(concat,registersTable[3]);
		reset_value_only(registersTable[0]);
		add(registersTable[2]);
		store_bez_inc(concat,registersTable[1]);


	}
	else if(expr->type==_VAR && expr->name!="temp")
	{
		//std::cout << expr->name << std::endl;
		reset(registersTable[0]);
		reset(registersTable[1]);
		reset(registersTable[2]);
		load(expr->name,registersTable[2]);
		swap(registersTable[2]);
		load(variable->name,registersTable[1]);
		reset_value_only(registersTable[0]);
		add(registersTable[2]);
		reset(registersTable[2]);
		store(variable->name,registersTable[2]);
	}
	else if(expr->type==VAL && expr->name=="temp")
	{
		reset(registersTable[1]);
		swap(registersTable[1]);
		reset(registersTable[2]);
		load(variable->name,registersTable[2]);
		reset_value_only(registersTable[0]);
		add(registersTable[1]);
		reset(registersTable[2]);
		store(variable->name,registersTable[2]);
	}
	else if(expr->type==VAL && expr->name!="temp")
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
				break;
			}
		}
		reset_value_only(registersTable[0]);
		add(registersTable[1]);
		reset(registersTable[2]);
		store(variable->name,registersTable[2]);
	}
	else
	{

	}
}
void *func_plus( var *v1, var *v2, int lineno)
{
	//liczba- 0, zmienna- 1
	if(v1->type==VAL && v2->type==VAL)
	{
		reset(registersTable[1]);
		reset(registersTable[2]);
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
		if(v2->value>0)
		{
			for(int i=0;i<v2->value;i++)
			{
				inc(registersTable[2]);
			}
		}
		else
		{
			for(int i=v2->value;i<0;i++)
			{
				dec(registersTable[2]);
			}
		}
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		v1->name="temp";
		v1->value=v1->value+v2->value;
		
	}
	else if(v1->type==VAL && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				reset(registersTable[2]);
				load(memory[i].name,registersTable[2]);
				swap(registersTable[2]);
				v2->value=memory[i].value;
			}
		}
		reset(registersTable[1]);
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
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		v1->type=_VAR;
		v1->name="temp";
		v1->value=v1->value+v2->value;
	}
	else if(v1->type==_VAR && v2->type==_VAR)
	{
		//NIE RUSZAĆ
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				reset(registersTable[1]);
				load(memory[i].name,registersTable[1]);
				swap(registersTable[1]);
				v1->value=memory[i].value;
				break;
			}
		}
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				reset(registersTable[2]);
				load(memory[i].name,registersTable[2]);
				swap(registersTable[2]);
				v2->value=memory[i].value;
				break;
			}
		}
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		v1->name="temp";
		v1->value=v1->value+v2->value;
	}
	else
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				reset(registersTable[2]);
				load(memory[i].name,registersTable[2]);
				swap(registersTable[2]);
				v1->value=memory[i].value;
			}
		}
		reset(registersTable[1]);
		if(v2->value>0)
		{
			for(int i=0;i<v2->value;i++)
			{
				inc(registersTable[2]);
			}
		}
		else
		{
			for(int i=v2->value;i<0;i++)
			{
				dec(registersTable[2]);
			}
		}
		reset(registersTable[0]);
		add(registersTable[1]);
		add(registersTable[2]);
		v1->name="temp";
		v1->value=v1->value+v2->value;
	}

}
void *func_minus( var *v1, var *v2, int lineno)
{
	//liczba- 0, zmienna- 1
	if(v1->type==VAL && v2->type==VAL)
	{
		reset(registersTable[1]);
		reset(registersTable[2]);
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
		if(v2->value>0)
		{
			for(int i=0;i<v2->value;i++)
			{
				inc(registersTable[2]);
			}
		}
		else
		{
			for(int i=v2->value;i<0;i++)
			{
				dec(registersTable[2]);
			}
		}
		reset(registersTable[0]);
		add(registersTable[1]);
		sub(registersTable[2]);
		v1->name="temp";
		v1->value=v1->value-v2->value;
		
	}
	else if(v1->type==VAL && v2->type==_VAR)
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				reset(registersTable[2]);
				load(memory[i].name,registersTable[2]);
				swap(registersTable[2]);
				v2->value=memory[i].value;
			}
		}
		reset(registersTable[1]);
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
		reset(registersTable[0]);
		add(registersTable[1]);
		sub(registersTable[2]);
		v1->type=_VAR;
		v1->name="temp";
		v1->value=v1->value-v2->value;
	}
	else if(v1->type==_VAR && v2->type==_VAR)
	{
		//NIE RUSZAĆ
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				reset(registersTable[1]);
				load(memory[i].name,registersTable[1]);
				swap(registersTable[1]);
				v1->value=memory[i].value;
				break;
			}
		}
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v2->name)
			{
				reset(registersTable[2]);
				load(memory[i].name,registersTable[2]);
				swap(registersTable[2]);
				v2->value=memory[i].value;
				break;
			}
		}
		reset(registersTable[0]);
		add(registersTable[1]);
		sub(registersTable[2]);
		v1->name="temp";
		v1->value=v1->value-v2->value;
	}
	else
	{
		for(int i=0;i<memory.size();i++)
		{
			if(memory[i].name==v1->name)
			{
				reset(registersTable[2]);
				load(memory[i].name,registersTable[2]);
				swap(registersTable[2]);
				v1->value=memory[i].value;
			}
		}
		reset(registersTable[1]);
		if(v2->value>0)
		{
			for(int i=0;i<v2->value;i++)
			{
				inc(registersTable[1]);
			}
		}
		else
		{
			for(int i=v2->value;i<0;i++)
			{
				dec(registersTable[1]);
			}
		}
		reset(registersTable[0]);
		add(registersTable[2]);
		sub(registersTable[1]);
		v1->name="temp";
		v1->value=v1->value-v2->value;
	}

}
void *func_times( var *v1, var *v2, int lineno)
{
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	load(v1->name,registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[0]);
	load(v2->name,registersTable[2]);
	swap(registersTable[2]);
	reset(registersTable[0]);
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
	put();

}
void *func_div(var *v1, var *v2, int lineno)
{
	//a->v1, b->v2, c->0,
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	load(v2->name,registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[0]);
	load(v1->name,registersTable[2]);
	reset(registersTable[2]);


	jzero(38);  
	swap(registersTable[1]);
	jzero(36);
	swap(registersTable[1]);

	jpos(13);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump else1)  ";
	swap(registersTable[1]);
	jpos(9);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump else12)  ";
	swap(registersTable[2]);
	sub(registersTable[2]);
	swap(registersTable[1]);
	swap(registersTable[2]);
	reset(registersTable[0]);
	sub(registersTable[2]);
	reset(registersTable[2]);
	jump(14);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump pos)  ";
	swap(registersTable[1]);
	jump(18);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump neg)  ";

	swap(registersTable[1]); command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (else1)  ";
	jpos(9);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump pos)  ";
	swap(registersTable[2]);
	sub(registersTable[2]);
	swap(registersTable[1]);
	swap(registersTable[2]);
	reset(registersTable[0]);
	sub(registersTable[2]);
	reset(registersTable[2]);
	jump(8);   command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump neg)  ";
	swap(registersTable[1]);


	/*jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	put();
	swap(registersTable[1]);
	put();
	swap(registersTable[2]);
	put();*/
	sub(registersTable[1]); command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (pos)  ";
	jneg(3);
	inc(registersTable[2]);
	jump(-3);
	swap(registersTable[2]);
	jump(8);

	add(registersTable[1]); command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (neg)  ";
	jneg(2);
	jump(3);
	dec(registersTable[2]);
	jump(-4);
	dec(registersTable[2]);


	swap(registersTable[2]); command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (kwkwk)  ";
	put();


	/*add(registersTable[1]);
	sub(registersTable[2]);
	jneg(0);
	swap(registersTable[1]);
	inc(registersTable[3]);*/


}
void *func_mod(var *v1, var *v2, int lineno)
{
	reset(registersTable[0]);
	reset(registersTable[1]);
	reset(registersTable[2]);
	reset(registersTable[5]);
	load(v2->name,registersTable[1]);
	swap(registersTable[5]);
	add(registersTable[5]);
	swap(registersTable[1]);
	reset(registersTable[0]);
	load(v1->name,registersTable[2]);
	reset(registersTable[2]);


	jzero(48);  
	swap(registersTable[1]);
	jzero(46);
	swap(registersTable[1]);

	jpos(13);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump else1)  ";
	swap(registersTable[1]);
	jpos(9);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump else12)  ";
	swap(registersTable[2]);
	sub(registersTable[2]);
	swap(registersTable[1]);
	swap(registersTable[2]);
	reset(registersTable[0]);
	sub(registersTable[2]);
	reset(registersTable[2]);
	jump(14);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump pos)  ";
	swap(registersTable[1]);
	jump(18);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump neg)  ";

	swap(registersTable[1]); command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (else1)  ";
	jpos(9);  command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump pos)  ";
	swap(registersTable[2]);
	sub(registersTable[2]);
	swap(registersTable[1]);
	swap(registersTable[2]);
	reset(registersTable[0]);
	sub(registersTable[2]);
	reset(registersTable[2]);
	jump(8);   command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (jump neg)  ";
	swap(registersTable[1]);


	/*jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	jump(1);
	put();
	swap(registersTable[1]);
	put();
	swap(registersTable[2]);
	put();*/
	sub(registersTable[1]); command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (pos)  ";
	jneg(3);
	inc(registersTable[2]);
	jump(-3);
	add(registersTable[1]);
	jump(9);

	add(registersTable[1]); command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (neg)  ";
	jneg(2);
	jump(3);
	dec(registersTable[2]);
	jump(-4);
	dec(registersTable[2]);


	swap(registersTable[2]); command_list[command_list.size()-1]=command_list[command_list.size()-1]+ "   (kwkwk)  ";
	swap(registersTable[2]);
	swap(registersTable[5]);
	jpos(4);

	reset(registersTable[0]);
	sub(registersTable[5]);
	jump(2);



	swap(registersTable[5]);
	reset(registersTable[4]);
}
void *condition_eq(var *a,var *b, int lineno)
{
	condition cond;
	cond.name="EQ";
	cond.pre_cond_index=command_list.size()-1;
	reset(registersTable[0]);
	reset(registersTable[1]);
	load(a->name,registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[2]);
	load(b->name,registersTable[2]);
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
	load(a->name,registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[2]);
	load(b->name,registersTable[2]);
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
	load(a->name,registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[2]);
	load(b->name,registersTable[2]);
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
	load(a->name,registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[2]);
	load(b->name,registersTable[2]);
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
	load(a->name,registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[2]);
	load(b->name,registersTable[2]);
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
	load(a->name,registersTable[1]);
	swap(registersTable[1]);
	reset(registersTable[2]);
	load(b->name,registersTable[2]);
	swap(registersTable[2]);
	reset(registersTable[0]);
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
		command_list[repeat_ending.post_cond_index]="JZERO " + std::to_string(j);
	}
	else if(repeat_ending.name=="NEQ")
	{
		command_list[repeat_ending.post_cond_index]="JNEG " + std::to_string(j);
		command_list.insert(command_list.begin()+repeat_ending.post_cond_index,"JPOS " + std::to_string(j+1));
	}
	else if(repeat_ending.name=="LE")
	{
		command_list[repeat_ending.post_cond_index]="JNEG " + std::to_string(j);
	}
	else if(repeat_ending.name=="GE")
	{
		command_list[repeat_ending.post_cond_index]="JPOS " + std::to_string(j);
	}
	else if(repeat_ending.name=="LEQ")
	{
		command_list[repeat_ending.post_cond_index]="JNEG " + std::to_string(j);
		command_list.insert(command_list.begin()+repeat_ending.post_cond_index,"JZERO " + std::to_string(j+1));
	}
	else if(repeat_ending.name=="GEQ")
	{
		command_list[repeat_ending.post_cond_index]="JPOS " + std::to_string(j);
		command_list.insert(command_list.begin()+repeat_ending.post_cond_index,"JZERO " + std::to_string(j+1));
	}
}

void for_to(std::string name, var *v1, var *v2, int lineno)
{
	memoryCells cell;
	cell.name=name;
	cell.value=v1->value;
	memory.push_back(cell);
	variable iter;
	iter.name=name;
	iter.value=v1->value;
	iter.type=VAL;
	iterators.push_back(iter);
	assign(&iter,v1,lineno);
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
	memory.push_back(cell);
	variable iter;
	iter.name=name;
	iter.value=v1->value;
	iter.type=VAL;
	iterators.push_back(iter);
	assign(&iter,v1,lineno);
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
	reset(registersTable[2]);
	store(iter.name,registersTable[2]);
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
	reset(registersTable[2]);
	store(iter.name,registersTable[2]);
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
