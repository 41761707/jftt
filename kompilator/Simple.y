%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "ST.h" 
	#include "SM.h" 
	#include "CG.h" 
	#define YYDEBUG 1 
	int errors;
	struct lbs
	{
		int for_goto;
		int for_jmp_false;
	};
	struct lbs * newlblrec()
	{
		return (struct lbs *) malloc(sizeof(struct lbs));
	}
	install ( char *sym_name )
	{
		symrec *s;
		s = getsym (sym_name);
		if (s == 0)
		{
			s = putsym (sym_name);
		}
		else 
		{ 
			errors++;
			printf( "%s is already defined\n", sym_name );
		}
	}
	context_check( enum code_ops operation, char *sym_name )
	{ 
		symrec *identifier;
		identifier = getsym( sym_name );
		if ( identifier == 0 )
		{ 
			errors++;
			printf( "%s", sym_name );
			printf( "%s\n", " is an undeclared identifier" );
		}
		else 
		{
			gen_code( operation, identifier->offset );
		}
	}
%}
%union sem_rec
{
	char *pidentifier;
	int num;
	char *id;
	struct lbs *lbls;
}
%start program
%token <num> num
%token <pidentifier> pidentifier
%token FOR
%token <lbls> IF WHILE REPEAT DO 
%token VAR BEGINNER END
%token THEN ELSE ENDIF FROM TO DOWNTO ENDFOR ENDWHILE ENDDO
%token UNTIL
%token READ WRITE       
%token LE GE LEQ GEQ EQ NEQ
%token PLUS MINUS TIMES DIV MOD
%token ASSIGN
%token ERROR 


%type value
%type <id> identifier
%type condition


%left ADD SUB
%left MUL DIV MOD

%%
program:
		VAR
			declarations
		BEGINNER							{gen_code(DATAx,data_location()-1);}
			commands
		END 								{gen_code(HALTx,0);YYACCEPT;}
		| BEGINNER							{gen_code(DATAx,data_location()-1);}
			commands
		END   								{gen_code(HALTx,0);YYACCEPT;}
;
declarations:
		declarations ',' pidentifier		{install($3);}
		|  pidentifier						{install($1);}
;
commands:
		commands command
		| command
;
command: 
		identifier ASSIGN expression ';'				{context_check(STOREx,$1);}
		|  IF condition 								{$1=(struct lbs *) newlblrec();$1->for_jmp_false=reserve_loc();}
		   THEN commands 								{$1->for_goto=reserve_loc();}
		   ELSE 										{back_patch($1->for_jmp_false,JMP_FALSEx,gen_label());}
		   commands 								
		   ENDIF										{back_patch($1->for_goto,GOTOx,gen_label());}
		|  IF condition 								{$1=(struct lbs *) newlblrec();$1->for_jmp_false=reserve_loc();}
		   THEN commands 								{$1->for_goto=reserve_loc();}
		   ENDIF										{back_patch($1->for_goto,GOTOx,gen_label());}
		|  WHILE 										{$1=(struct lbs *) newlblrec();$1->for_goto=gen_label();}
		   condition 									{$1->for_jmp_false=reserve_loc();}
		   DO 											
		   commands 
		   ENDWHILE										{gen_code(GOTOx,$1->for_goto); back_patch($1->for_jmp_false,JMP_FALSEx,gen_label());}
		|  READ  identifier ';'							{context_check(GET_I,$2);}
		|  WRITE value ';'								{gen_code(PUT_I,0);}
;
expression: 
		value							;
		|  value PLUS value				{gen_code(ADDx,0);}
		|  value MINUS value			{gen_code(SUBx,0);}
		|  value TIMES value			{gen_code(MULTx,0);}
		|  value DIV value				{gen_code(DIVx,0);}
;
condition:
		value EQ value					{gen_code(EQx,0);}
		|  value LE value				{gen_code(LTx,0);}
		|  value GE value				{gen_code(GTx,0);}
;
value:
		num								{gen_code(LD_INTx,$1);}
		|  identifier 					{context_check(LD_VARx,$1);}
identifier: 
		pidentifier  					{}
		|  pidentifier'['pidentifier']' {}
		|  pidentifier'['num']'         {}
;
%%

main( int argc, char *argv[] )
{ 
	extern FILE *yyin;
	++argv; --argc;
	yyin = fopen( argv[0], "r" );
	yydebug = 1;
	errors = 0;
	yyparse ();
	printf ( "Parse Completed\n" );
	if(errors==0)
	{ 
		print_code ();
		fetch_execute_cycle();
	}
}

yyerror(char *s)
{
	errors++;
	printf ("%s\n", s);
}