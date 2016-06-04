#!/bin/bash

for f in $(ls test/*.oc | sed 's/.*\/\(.*\)\.oc/\1/'); do
    echo -n $f
    ./oc test/${f}.oc > /dev/null 2>&1 
    if (($?)); then 
        echo '    OC FAILURE'
    else 
        ./compile.bash $f > /dev/null 2>&1
        if (($?)); then 
            echo '    GCC FAILURE'
        else
            echo
        fi
    fi
done
     

