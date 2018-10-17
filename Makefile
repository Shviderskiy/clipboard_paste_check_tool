all:
	gcc -std=c99 -O2 -pedantic -Wall -Wextra -s -nostdlib -mwindows main.c -lkernel32 -luser32 -o cpct.exe
