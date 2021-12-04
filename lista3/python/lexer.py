import ply.lex as lex


tokens = { NAME, NUMBER, PLUS, TIMES, MINUS, DIVIDE, ASSIGN, LPAREN, RPAREN }
ignore = ' \t'

# Tokens
NAME = r'[a-zA-Z_][a-zA-Z0-9_]*'
NUMBER = r'\d+'

# Special symbols
PLUS = r'\+'
MINUS = r'-'
TIMES = r'\*'
DIVIDE = r'/'
ASSIGN = r'='
LPAREN = r'\('
RPAREN = r'\)'

# Ignored pattern
ignore_newline = r'\n+'

# Extra action for newlines
def ignore_newline(self, t):
    self.lineno += t.value.count('\n')

def error(self, t):
    print("Illegal character '%s'" % t.value[0])
    self.index += 1