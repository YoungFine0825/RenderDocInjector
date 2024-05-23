# Made by Sean P

all:
	mkdir -p build
	g++ -std=c++11 -masm=intel -c -I include src/AsmInject_x86.cpp -o build/AsmInject_x86.o

all64:
	mkdir -p build
	g++ -std=c++11 -masm=intel -m32 -c -I include src/AsmInject_x86.cpp -o build/AsmInject_x86.o

asm:
	mkdir -p asm
	g++ -std=c++11 -masm=intel -S -I include src/AsmInject_x86.cpp -o asm/AsmInject_x86.a

asm64:
	mkdir -p asm
	g++ -std=c++11 -masm=intel -m32 -S -I include src/AsmInject_x86.cpp -o asm/AsmInject_x86.a

clean:
	rm -rf build bin asm

