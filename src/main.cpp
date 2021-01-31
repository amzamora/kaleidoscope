#include <iostream>
#include <fstream>
#include <sstream>

#include "types.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char *argv[]) {
	//  Parse arguments
	if (argc != 2) {
		std::cout << "Usage: ./kaleidoscope file_path" << '\n' << '\n';
		throw std::exception();
	}

	// Read file
	std::ifstream in;
	in.open(argv[1]);
	std::stringstream stream;
	stream << in.rdbuf();
	std::string source_file = stream.str();

	// Lex file
	std::vector<Token> tokens = lex(source_file);
	// for (unsigned int i = 0; i < tokens.size(); i++) {
	// 	std::cout << tokens[i].literal << '\n';
	// }

	// Parse
	std::unique_ptr<ProgramAST> prg_ast = parse(tokens);
	print(prg_ast.get());

	return 0;
}
