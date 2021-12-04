%option noyywrap
%{
    #include "zad1.tab.hpp"
    int yylex();
    int powerCounter = 0;
%}

%x COMMENT
%x POWER
%x POWEREXP

%%
<*>\\\n                         ;
\#                             {BEGIN(COMMENT);};
<COMMENT>\n                    {BEGIN(INITIAL);};
[0-9]+                         {yylval = atoi(yytext); return VAL;}
(\+|\-|\*|\/|\n|\(|\))        {return yytext[0];}
\^[[:blank:]]*                {BEGIN(POWER); return '^';}
<POWER>\(                       {BEGIN(POWEREXP); powerCounter++; return yytext[0];}
<POWER>(\-)?[0-9]+                 {BEGIN(INITIAL); yylval = atoi(yytext); return POW;}
<POWEREXP>[0-9]+                  {yylval = atoi(yytext); return POW;}
<POWEREXP>\-                       {return INDMINUS;}
<POWEREXP>\(                    {powerCounter++; return '(';}
<POWEREXP>\)                    {powerCounter--; if (powerCounter == 0) {BEGIN(INITIAL);}; return ')';}
<*>.                           ;
%%