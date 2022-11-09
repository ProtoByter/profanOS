run:
	gcc -c main.c -o main.o -g -ffreestanding -Wno-unused -Wall -Wextra -fno-exceptions -m32 -I ./../..
	ld -m elf_i386 -e main -o main.pe ./main.o
	objcopy -O binary main.pe main.bin -j .text -j .data -j .rodata -j .bss
	make clean

fullclean:
	make clean
	rm -Rf ./*.bin

clean:
	rm -Rf ./*.full
	rm -Rf ./*.o
	rm -Rf ./*.pe