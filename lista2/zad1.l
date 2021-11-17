%{
	#include<stdio.h>
	#include <fcntl.h>
	int yylex();
	int yywrap();
	int wordCount=0;
	int lineCount=0;



%}


%%
^[ \t]+(\n)?    ;
[ \t]+(\n)?$    ;
[ \t]+    		{printf(" ");}
[a-zA-z]+   	{ECHO;wordCount=wordCount+1;}
\n+         	{printf("\n");lineCount=lineCount+1;}

%%

int yywrap(){}

int main(int argc, char *argv[])
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
    printf("\n");
	printf("Liczba slow: %d\n",wordCount);
	printf("Liczba linii: %d",lineCount);
    return 0;
}
