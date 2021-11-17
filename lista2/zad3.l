%{
        #include <stdio.h>
        #include <fcntl.h>
        int yylex();
        int yywrap();

        int docs=0;

%}

%x commentMulti
%x commentSingle
%x commentMultiDOC
%x commentSingleDOC
%x ATTRIBUTE
%x INCLUDE

%%

\"              {ECHO;BEGIN(ATTRIBUTE);}
<ATTRIBUTE>{
\"              { ECHO; BEGIN(INITIAL); } 
.               ECHO;
}

"#include"      {ECHO;BEGIN(INCLUDE);}
<INCLUDE>\"     {ECHO;BEGIN(ATTRIBUTE);}
<INCLUDE>\>     {ECHO;BEGIN(INITIAL);}

"/**"|"/*!"                      {if(docs) ECHO;BEGIN(commentMultiDOC);}
<commentMultiDOC>[^*\n]*         {if(docs) ECHO;}
<commentMultiDOC>"*"+[^*/\n]*    {if(docs) ECHO;}
<commentMultiDOC>\n              {if(docs) ECHO;}
<commentMultiDOC>"*"+"/"         {if(docs) ECHO;BEGIN(INITIAL);}



"/*"                         BEGIN(commentMulti);
<commentMulti>[^*\n]*        ;
<commentMulti>"*"+[^*/\n]*   ;
<commentMulti>\n             ;
<commentMulti>"*"+"/"        BEGIN(INITIAL);

"//"                         BEGIN(commentSingle);
<commentSingle>.             ;
<commentSingle>\n            BEGIN(INITIAL);

"///"|"//!"                  {if(docs) ECHO;BEGIN(commentSingleDOC);}
<commentSingleDOC>.             {if(docs) ECHO;}
<commentSingleDOC>\n            {if(docs) ECHO;BEGIN(INITIAL);}


%%


int yywrap(){}

int main(int argc, char **argv)
{
    yyin = fopen(argv[1], "r");
    char *out=argv[2];
    if(argc>3)
    {
        docs=1;
    }
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