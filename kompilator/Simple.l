
%{
#include <string.h>
#include <stdlib.h> 
#include "Simple.tab.h"
%}
NUM            -?([0-9]+)
PIDENTIFIER    [_a-z]+


%%
[ \t]+          ;
\n              ;
"ASSIGN"          {return(ASSIGN);}
"VAR"             {return(VAR);}
"BEGIN"        	  {return(BEGINNER);}
"END"             {return(END);}

"PLUS"            {return(PLUS);}
"MINUS"           {return(MINUS);}
"TIMES"           {return(TIMES);}
"DIV"             {return(DIV);}
"MOD"             {return(MOD);}

"EQ"              {return(EQ);}
"NEQ"             {return(NEQ);}
"LE"              {return(LE);}
"GE"              {return(GE);}
"LEQ"             {return(LEQ);}
"GEQ"             {return(GEQ);}

"WHILE"           {return(WHILE);}
"ENDWHILE"        {return(ENDWHILE);}

"FOR"             {return(FOR);}
"FROM"            {return(FROM);}
"TO"              {return(TO);}
"DO"              {return(DO);}
"ENDFOR"          {return(ENDFOR);}
"DOWNTO"          {return(DOWNTO);}

"REPEAT"          {return(REPEAT);}
"UNTIL"           {return(UNTIL);}

"READ"            {return(READ);}
"WRITE"           {return(WRITE);}

"IF"              {return(IF);}
"THEN"            {return(THEN);}
"ELSE"            {return(ELSE);}
"ENDIF"           {return(ENDIF);}

{PIDENTIFIER}     { yylval.id = (char *) strdup(yytext);return(pidentifier); }
{NUM}		        { yylval.num = atoi( yytext );return(num); }
\(.*\)          ;
.               return(yytext[0]);



%%
int yywrap(void){}