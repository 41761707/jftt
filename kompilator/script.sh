lex Simple.l
bison -d Simple.ypp
g++ -o main Simple.tab.cpp Simple.tab.hpp lex.yy.c 2> /dev/null
./main test_simple3.txt result.txt
./maszyna-wirtualna result.txt
