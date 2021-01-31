all:
	c++ src/main.cpp src/lexer.cpp  src/parser.cpp src/types.cpp -o kaleidoscope -std=c++17

clean:
	rm -f kaleidoscope
