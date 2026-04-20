#!/bin/bash

MACRO=""

if [[ -n "$2" ]]; then
    MACRO+="-D$2 "
    # echo $MACRO
fi

if [[ -n "$3" ]]; then
    MACRO+="-D$3 "
    # echo $MACRO
fi

g++ $MACRO main.cpp render.cpp $1.cpp -o $1 -mavx2 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -O3 -DNDEBUG -march=native

./$1

rm $1
