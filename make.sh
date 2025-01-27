#!/bin/sh

# Compile all C files into an executable named 'main'
gcc main.c plansza.c wejsciewyjscie.c utils.c wejsciewyjscie.h plansza.h utils.h -o saper

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. You can run the program using ./main"
else
    echo "Compilation failed."
fi