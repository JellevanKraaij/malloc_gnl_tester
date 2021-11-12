#!/bin/bash

DEBUG=1;

for i in $(seq 1 5) $(seq 10 10 100)
do
	echo -n "Small File Running buffersize $i";
	gcc -fsanitize=address -g3 -D PRINT_ON=$DEBUG -D BUFFER_SIZE=$i -D FILE_NAME=\"test1.txt\" test.c ../get_next_line.c ../get_next_line_utils.c && ./a.out && echo -e '\033[0;32m' " OK " '\033[0m' || { echo -e '\033[0;31m' " ERROR " '\033[0m'; exit; }
done

for i in $(seq 1 5) $(seq 10 10 100)
do
	echo -n "Big File Running buffersize $i";
	gcc -fsanitize=address -g3 -D PRINT_ON=$DEBUG -D BUFFER_SIZE=$i -D FILE_NAME=\"test2.txt\" test.c ../get_next_line.c ../get_next_line_utils.c && ./a.out && echo -e '\033[0;32m' " OK " '\033[0m'|| { echo -e '\033[0;31m' " ERROR " '\033[0m'; exit; }
done

echo -n "Invalid file descriptor"
gcc -fsanitize=address -g3 -D PRINT_ON=$DEBUG -D BUFFER_SIZE=$i -D FILE_NAME=\"NON_EXISTING\" test.c ../get_next_line.c ../get_next_line_utils.c && ./a.out && echo -e '\033[0;32m' " OK " '\033[0m' || echo -e '\033[0;31m' " ERROR " '\033[0m'