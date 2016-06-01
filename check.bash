#!/bin/bash

for f in $(ls test/*.oc); do
    echo -n $f
    ./oc $f > /dev/null 2>&1 
    if (($?)); then 
        echo '    FAILURE'
    else 
        echo
    fi
done
     

