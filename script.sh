#!/bin/bash

MACRO=""
if [[ -n "$2" ]]; then
    MACRO="-D$2"
fi

g++ $MACRO main.cpp render.cpp $1.cpp -o $1 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -O3 -DNDEBUG -march=native

./$1

rm $1
