run:
	rm -f program.exe
	g++ -g -std=c++11 -Wall application.cpp  -o program.exe
	./program.exe
	