#!/bin/bash
echo "Compiling Code..."
g++ long_assignment.cpp -o runcode | tr -d '\r'
echo "Code Compiled Succesfully"
echo "To Run,Use ./runcode in Terminal"