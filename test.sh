#!/bin/bash

echo "Created by alphawong"

echo "Building ..."
gcc -o assignment2.out ./assignment2.c
echo

echo "Testing ..."
./assignment2.out ./test.txt
echo

echo "Removeing the binary"
rm ./assignment2.out
echo

echo "Completed"

