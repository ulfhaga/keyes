#!/bin/bash
set -x
gcc -o keyes keyes.c -lwiringPi -llirc_client
cp -f keyes    ~/bin/keyes 
file ~/bin/keyes 

