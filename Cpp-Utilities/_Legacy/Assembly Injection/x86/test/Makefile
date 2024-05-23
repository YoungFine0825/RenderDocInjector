# Created by Sean P

all: example_program
	# This should be used when compiling on 32-bit systems.

all64: example_program_on_x64
	# This should be used when compiling on 64-bit systems.
	# NOTE: Make sure the following packages are installed: gcc-multilib g++-multilib

run: example_program
	./bin/example

run64: example_program_on_x64
	./bin/example
	
example_program: compile_asm_inject_x86_funcs
	mkdir -p bin
	g++ -std=c++11 -masm=intel -I include -I ../include -I "../../System Utilities/include" "../../System Utilities/src/SP_SysUtils.cpp" src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o bin/example

example_program_on_x64: compile_asm_inject_x86_funcs_on_x64
	mkdir -p bin
	g++ -std=c++11 -masm=intel -m32 -I include -I ../include -I "../../System Utilities/include" "../../System Utilities/src/SP_SysUtils.cpp" src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o bin/example

example_asm: compile_asm_inject_x86_funcs
	mkdir -p asm
	g++ -std=c++11 -masm=intel -S -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o asm/example.a

example_asm_on_x64: compile_asm_inject_x86_funcs_on_x64
	mkdir -p asm
	g++ -std=c++11 -masm=intel -S -m32 -I include -I ../include src/Example_AsmInject_x86.cpp build/AsmInject_x86.o -o asm/example.a

compile_asm_inject_x86_funcs:
	mkdir -p build
	g++ -std=c++11 -masm=intel -c -I ../include ../src/AsmInject_x86.cpp -o build/AsmInject_x86.o

compile_asm_inject_x86_funcs_on_x64:
	mkdir -p build
	g++ -std=c++11 -masm=intel -m32 -c -I ../include ../src/AsmInject_x86.cpp -o build/AsmInject_x86.o

clean:
	rm -rf build bin asm

