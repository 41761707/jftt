all: lex.yy.c Simple.tab.hpp Simple.tab.cpp main

lex.yy.c:
	lex Simple.l 
Simple.tab.hpp Simple.tab.cpp:
	bison -d Simple.ypp
main:
	g++ -Wall -Wextra -std=c++11 -pedantic Simple.tab.cpp Simple.tab.hpp lex.yy.c -o main
