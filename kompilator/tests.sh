lex Simple.l
bison -d Simple.ypp
g++ -o main Simple.tab.cpp Simple.tab.hpp lex.yy.c 2> /dev/null
./main 0-div-mod.imp result.txt
./maszyna-wirtualna result.txt
./main 1-numbers.imp result.txt
./maszyna-wirtualna result.txt
./main 2-fib.imp result.txt
./maszyna-wirtualna result.txt
./main 3-fib-factorial.imp result.txt
./maszyna-wirtualna result.txt
./main 4-factorial.imp result.txt
./maszyna-wirtualna result.txt
./main 5-tab.imp result.txt
./maszyna-wirtualna result.txt
./main 6-mod-mult.imp result.txt
./maszyna-wirtualna result.txt
./main 7-loopiii.imp result.txt
./maszyna-wirtualna result.txt
./main 8-for.imp result.txt
./maszyna-wirtualna result.txt
./main 9-sort.imp result.txt
./maszyna-wirtualna result.txt
./main 10-div-mod.imp result.txt
./maszyna-wirtualna result.txt
./main 11-test-0.imp result.txt
./maszyna-wirtualna result.txt
./main 12-test-1a.imp result.txt
./maszyna-wirtualna result.txt
./main 13-test-1b.imp result.txt
./maszyna-wirtualna result.txt
./main 14-test-1c.imp result.txt
./maszyna-wirtualna result.txt
./main 15-test-1d.imp result.txt
./maszyna-wirtualna result.txt
./main 16-test-2.imp result.txt
./maszyna-wirtualna result.txt