import ply.yacc as yacc
import math

# Get the token map from the lexer.  This is required.
import calclex
from calclex import tokens
from calclex import precedence

def field_property(number,n):
    return ((number % n) + n ) % n

def field_pow(a,b,n):
    result=1
    for i in range(b):
        result=(result*a)%n
    return result

def field_mult(a,b,n):
    result=0
    for i in range(b):
        result=(result+(a%n))%n
    return result

def inverse(a, n):
    for x in range(n):
        if (a * x) % n == 1:
            return x
def p_statement_expr(p):
    'statement : expression'
    print("\n")
    print("Wynik: ",p[1])

def p_expression_plus(p):
    'expression : expression PLUS term'
    p[0] = field_property(p[1]+p[3],1234577)
    print('+ ', end='')

def p_expression_minus_expression(p):
    'expression : expression MINUS expression '
    p[0] = field_property(p[1]-p[3],1234577)
    print('- ', end='')

def p_expr_uminus(p):
    'expression : MINUS expression %prec UMINUS'
    p[0] = field_property(-p[2],1234577)
    print("~ ", end='')

def p_expression_term(p):
    'expression : term'
    p[0] = field_property(p[1],1234577)

def p_term_times(p):
    'term : term TIMES factor'
    p[0] = field_property(p[1]*p[3],1234577)
    print("* ", end='')

def p_term_div(p):
    'term : term DIVIDE factor'
    if(p[3]==0):
        pass
        #TODO-error
    else:
        p[3]=inverse(p[3],1234577)
        p[0] = field_mult(p[1],p[3],1234577)
    print("/ ", end='')

def p_term_factor(p):
    'term : factor'
    p[0] = field_property(p[1],1234577)

def p_factor_num(p):
    'factor : NUMBER'
    p[0] = field_property(p[1],1234577)
    print(str(p[0])+" ", end='')

def p_factor_expr(p):
    'factor : LPAREN expression RPAREN'
    p[0] = field_property(p[2],1234577)

def p_expression_pow(p):
    'expression : expression POW term'
    p[0]=field_pow(p[1],p[3],1234577)
    print("^ ", end="")
def p_expression_pow_minus(p):
    'expression : expression POW MINUS term'
    p[4]=-p[4]
    p[4]=field_property(p[4],1234577)
    p[0]=field_pow(p[1],p[4]-1,1234577)
    print("~ ^ ", end="")
def p_expression_div_minus(p):
    'term : term DIVIDE MINUS factor'
    p[4]=-p[4]
    p[4]=inverse(p[4],1234577)
    p[0] = field_mult(p[1],p[4],1234577)
    print("~ / ", end="")
def p_expression_times_minus(p):
    'term : term TIMES MINUS factor'
    p[4]=-p[4]
    p[4]=field_property(p[4],1234577)
    p[0]=field_property(p[1]*p[4],1234577)
    print("~ * ", end="")

def p_error(p):
    print("Blad")
    

parser = yacc.yacc()

while True:
    userinput = ''
    line = input('')
    while line != '':
        userinput += line
        line = input()
    result = parser.parse(userinput)