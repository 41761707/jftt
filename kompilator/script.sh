lex Simple.l
bison -d Simple.ypp
g++ -o main Simple.tab.cpp Simple.tab.hpp lex.yy.c
./main test_simple4.txt result.txt
./maszyna-wirtualna result.txt
