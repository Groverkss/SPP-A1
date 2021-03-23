#!/bin/bash

# gcc -O0 -std=c99 -Wall $1

gcc -O2 -std=c99 -Wall -S -o dump.S $1
