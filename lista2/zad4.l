%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    int yylex();
    int yywrap();

    int MAXSIZE = 24;       
    int stack[24];     
    int top = -1;       

    int power(int base,int exponent) 
    {
        int result = 1;
        for (int i = 0; i < exponent; i++) 
        {
            result *= base;
        }
        return result;
    }     

    int isempty() 
    {

       if(top == -1)
       {
          return 1;
       }
       else
       {
          return 0;
       }
    }
       
    int isfull() 
    {

       if(top == MAXSIZE)
       {
          return 1;
       }
       else
       {
          return 0;
       }
    }

    int peek() 
    {
       return stack[top];
    }

    int pop() 
    {
       int data;
        
       if(!isempty()) 
       {
          data = stack[top];
          top = top - 1;   
          return data;
       } 
       else 
       {
          printf("Could not retrieve data, Stack is empty.\n");
       }
    }

    int push(int data) 
    {

       if(!isfull()) 
       {
          top = top + 1;   
          stack[top] = data;
       } 
       else 
       {
          printf("Could not insert data, Stack is full.\n");
       }
    }
%}


%x ERR


%%

-?[0-9]+       {

    push(atoi(yytext));

}
[ \t]          ;
\+             {
    if(top<1)
    {
        printf("Za mało liczb\n");
        BEGIN(ERR);
    }
    else
    {
        int second=pop();
        int first=pop();
        push(first+second);
    }

}
\-             {

    if(top<1)
    {
        printf("Za mało liczb\n");
        BEGIN(ERR);
    }
    else
    {
        int second=pop();
        int first=pop();
        push(first-second);
    }

}
\*             {

    if(top<1)
    {
        printf("Za mało liczb\n");
        BEGIN(ERR);
    }
    else
    {
        int second=pop();
        int first=pop();
        push(first*second);
    }

}
\/             {
    int second=pop();
    int first=pop();
    if(top<-1)
    {
        printf("Za mało liczb\n");
        BEGIN(ERR);
    }
    else if(second==0)
    {
        printf("Nie można dzielić przez 0!\n");
        BEGIN(ERR);
    }
    else
    {
        push(first/second);
    }

}
\%             {
    int second=pop();
    int first=pop();
    if(top<-1)
    {
        printf("Za mało liczb\n");
        BEGIN(ERR);
    }
    else if(second==0)
    {
        printf("Nie można dzielić przez 0!\n");
        BEGIN(ERR);
    }
    else
    {
        push(first%second);
    }

}
\^             {

    int exponent=pop();
    if(top<0)
    {
        printf("Za mało liczb\n");
        BEGIN(ERR);
    }
    else if(exponent<0)
    {
        printf("Program nie przewiduje ujemnych poteg\n");
    }
    else
    {
        int result=power(pop(),exponent);
        push(result);
    }


}
\n             {

    if(top==0)
    {
        printf("%d ",pop());
        printf("\n");
    }
    else if(top>0)
    {
        printf("Za malo operatorow\n");
        while(!isempty())
        {
            int data=pop();
        }
    }
    else
    {
        printf("Za malo liczb\n");
        while(!isempty())
        {
            int data=pop();
        }
    }
}

[^[:blank:]]   {printf("Niedozwolony znak: %s\n",yytext);BEGIN(ERR);}

<ERR>.*     ;
<ERR>\n     {
    while(!isempty())
    {
        int data=pop();
    }
    BEGIN(INITIAL);
}




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