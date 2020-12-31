#include <iostream>

#include "types.hpp"

std::unique_ptr<ProgramAST> parse(std::vector<Token> tokens) {
	for (auto it = tokens.begin(); it != tokens.end(); it++) {
		std::cout << (*it).literal << '\n';
	}

	return nullptr;
}
