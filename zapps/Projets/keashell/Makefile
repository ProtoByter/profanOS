run:
	gcc -c main.c -o main.o -g -ffreestanding -Wno-unused -Wall -Wextra -fno-exceptions -m32 -I ./../..
	gcc -c src/lexer.c -o lexer.o -g -ffreestanding -Wno-unused -Wall -Wextra -fno-exceptions -m32 -I ./../..
	gcc -c src/parser.c -o parser.o -g -ffreestanding -Wno-unused -Wall -Wextra -fno-exceptions -m32 -I ./../..
	gcc -c src/interpretor.c -o interpretor.o -g -ffreestanding -Wno-unused -Wall -Wextra -fno-exceptions -m32 -I ./../..
	ld -m elf_i386 -e main -o main.pe ./main.o ./lexer.o ./parser.o ./interpretor.o
	objcopy -O binary main.pe main.bin -j .text -j .data -j .rodata -j .bss
	make clean

fullclean:
	make clean
	rm -Rf ./*.bin

clean:
	rm -Rf ./*.full
	rm -Rf ./*.o
	rm -Rf ./*.pe