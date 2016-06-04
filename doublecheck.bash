#!/bin/bash

files="00-trivial 01-hello 03-test3 04-test4 06-test6 07-assert 10-hundred.oC 11-numbers 12-elseif 13-assertfail 14-ocecho 20-fib-array 21-eratosthenes 23-atoi 30-fac-fnloop 31-fib-2supn 40-arraystack 41-linkedstack 42-viiiqueens 44-dot-product 45-towers-of-hanoi 53-insertionsort"

for f in $files; do
    echo -e "\n\n\n$f\n"
    ./oc test/${f}.oc
    ./compile.bash ${f}
    echo -e '\n'
    ./${f}.out
done
