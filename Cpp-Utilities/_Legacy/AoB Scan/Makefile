# Made by Sean P

all:
	mkdir -p build
	g++ -std=c++11 -c -I include -I "../System Utilities/include" src/SP_AoB_Scan.cpp -o build/SP_Aob_Scan.o

all32:
	mkdir -p build
	g++ -std=c++11 -c -m32 -I include -I "../System Utilities/include" src/SP_AoB_Scan.cpp -o build/SP_Aob_Scan.o
	
clean:
	rm -rf bin build asm test/bin test/build test/asm
