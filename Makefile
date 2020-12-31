all:
	c++ -std=c++17 src/main.cpp src/lexer.cpp -o kaleidoscope

clean:
	rm -f kaleidoscope
