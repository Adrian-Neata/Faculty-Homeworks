tema1: tema1.asm
	nasm -f elf32 -o tema1.o $<
	gcc -m32 -o $@ tema1.o

clean:
	rm -f tema1 tema1.o
