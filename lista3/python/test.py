tokens = (
'NUMBER', 'POW','NEGPOW','COMMENT'
)

literals = ['=', '+', '-', '*', '/', '(', ')']

# Tokens
t_NEGPOW  = r'\^-'
t_POW     = r'\^(-?)'

states = (('ccode','exclusive'),)

def t_NUMBER(t):
    r'\d+'
    t.value = int(t.value)
    return t
def t_ccode_COMMENT(t):
    r'\#[:alpha:]+|\+|\-|\/|\*|\)'
    return t
def t_ccode(t):
     r'\#'
     t.lexer.code_start = t.lexer.lexpos        
     t.lexer.level = 1                          
     t.lexer.begin('ccode')    

def t_ccode_extend(t):
    r'\\\n'
    t.value = t.lexer.lexdata[t.lexer.lexpos:t.lexer.lexpos+1]
    t.type = "COMMENT"
    return t             


def t_ccode_end(t):
    r'\n'
    t.lexer.level -=1
    t.value = t.lexer.lexdata[t.lexer.lexpos:t.lexer.lexpos+1]
    if t.lexer.level == 0:
        t.type = "COMMENT"
        t.lexer.begin('INITIAL')           
        return t
    else:
        t.type = "COMMENT"
        return t


def t_ccode_error(t):
     t.lexer.skip(1)

t_ANY_ignore = " \t"

def t_newline(t):
    r'\n+'
    t.lexer.lineno += t.value.count("\n")

def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)

# Build the lexer
import ply.lex as lex
lexer = lex.lex()

# Parsing rules

precedence = (
    ('left', '+', '-'),
    ('left', '*', '/'),
    ('right','POW'),
    ('right', 'UMINUS'),
)
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


def p_expression_binop(p):
    '''expression : expression '+' expression
                  | expression '-' expression
                  | expression '*' expression
                  | expression '/' expression'''
    if p[2] == '+':
        p[0] = field_property(p[1]+p[3],1234577)
        print('+ ', end='')
    elif p[2] == '-':
        p[0] = field_property(p[1]-p[3],1234577)
        print('- ', end='')
    elif p[2] == '*':
        p[0] = field_property(p[1]*p[3],1234577)
        print("* ", end='')
    elif p[2] == '/':
        p[3]=inverse(p[3],1234577)
        p[0] = field_mult(p[1],p[3],1234577)
        print("/ ", end='')

'''
def p_expression_pow_minus(p):
    'expression : expression NEGPOW expression'
    p[3]=-p[3]
    p[3]=field_property(p[3],1234577)
    p[0]=field_pow(p[1],p[3]-1,1234577)
    print("~ ^ ", end="")
'''


def p_expression_pow(p):
    'expression : expression POW expression'
    if(p[2]=='^'):
        print(p[3])
        if(int(p[3])<0):
            print("SIEMA")
            p[3]=-p[3]
            p[3]=field_property(p[3],1234577)
            p[0]=field_pow(p[1],p[3]-1,1234577)
        else:
            p[3]=field_property(p[3],1234577)
            p[0]=field_pow(p[1],p[3],1234577)
        print("^ ", end="")
    elif(p[2]=='^-'):
        p[3]=-p[3]
        p[3]=field_property(p[3],1234577)
        p[0]=field_pow(p[1],p[3]-1,1234577)
        print("~ ^ ", end="")



def p_expression_uminus(p):
    "expression : '-' expression %prec UMINUS"
    p[0] = field_property(-p[2],1234577)
    print("~ ", end='')


def p_expression_group(p):
    "expression : '(' expression ')'"
    'factor : LPAREN expression RPAREN'
    p[0] = field_property(p[2],1234577)


def p_expression_number(p):
    "expression : NUMBER"
    p[0] = field_property(p[1],1234577)
    print(str(p[0])+" ", end='')



def p_error(p):
    if p:
        if(p.type=="COMMENT"):
            pass
        else:
            print("Syntax error at '%s'" % p.value)
    else:
        print("Syntax error at EOF")

import ply.yacc as yacc
parser = yacc.yacc()
while True:
    try:
        s = input('calc > ')
    except EOFError:
        break
    if not s:
        continue
    s=s+"\n"
    yacc.parse(s)