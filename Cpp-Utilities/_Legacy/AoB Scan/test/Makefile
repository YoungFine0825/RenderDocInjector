# Made by Sean P

all:
	mkdir -p bin
	g++ -std=c++11 -I include -I ../include -I "../../System Utilities/include" -I ../../IO/include "../../System Utilities/src/SP_SysUtils.cpp" ../../IO/src/SP_Shell_IO.cpp ../src/SP_AoB_Scan.cpp src/SP_AoB_Scan_Example.cpp -o bin/example

run: all
	./bin/example

all32:
	mkdir -p bin
	g++ -std=c++11 -m32 -I include -I ../include -I "../../System Utilities/include" -I ../../IO/include "../../System Utilities/src/SP_SysUtils.cpp" ../../IO/src/SP_Shell_IO.cpp ../src/SP_AoB_Scan.cpp src/SP_AoB_Scan_Example.cpp -o bin/example

run32: all32
	./bin/example

clean:
	rm -rf bin build asm
