%{
	#include <stdio.h>
	#include <fcntl.h>
	int yylex();
	int yywrap();

	char* rollback="";

	char* concat(const char* text1, const char* text2) 
	{
        char* result = malloc(strlen(text1) + strlen(text2) + 1);
        
        strcpy(result, text1);
        strcat(result, text2);
        
        return result;
    }

%}


%x COMMENT
%x CDATA
%x ATTRIBUTE

%%

[a-zA-z]+   	{ECHO;}

\"              {ECHO;BEGIN(ATTRIBUTE);}
<ATTRIBUTE>{
\"              { ECHO; BEGIN(INITIAL); } 
.               ECHO;
}

\<!\[CDATA\[    {ECHO;BEGIN(CDATA);}
<CDATA>.|\n		{ECHO;}
<CDATA>\]\]\>	{ECHO;BEGIN(INITIAL);}

\<!--			{rollback="<!--";BEGIN(COMMENT);}
<COMMENT>.|\n   {rollback=concat(rollback,yytext);}
<COMMENT>-- 	{printf("%s--",rollback);rollback="";BEGIN(INITIAL);}
<COMMENT>--\>	{BEGIN(INITIAL);}


%%

int yywrap(){}

int main(int argc, char **argv)
{
	yyin = fopen(argv[1], "r");
    char *out=argv[2];
    int fd = open(out, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("open failed");
        exit(1);
    }

    if (dup2(fd, 1) == -1) {
        perror("dup2 failed"); 
        exit(1);
    }
    yylex();
    fclose(yyin);
    return 0;
}