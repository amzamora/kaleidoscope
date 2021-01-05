#include <iostream>

#include "types.hpp"

// Prototypes
// ----------
std::tuple<std::unique_ptr<FunctionAST>, std::vector<Token>::iterator> parse_function(std::vector<Token>::iterator it, std::vector<Token>::iterator end);
std::tuple<std::unique_ptr<PrototypeAST>, std::vector<Token>::iterator> parse_prototype(std::vector<Token>::iterator it, std::vector<Token>::iterator end);
std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_expression(std::vector<Token>::iterator it, std::vector<Token>::iterator end);

// Implementations
// ---------------

std::unique_ptr<ProgramAST> parse(std::vector<Token> tokens) {
	std::vector<std::unique_ptr<FunctionAST>> functions;
	std::vector<std::unique_ptr<ExprAST>> expressions;

	for (auto it = tokens.begin(); it != tokens.end(); it++) {
		if ((*it).literal == "def") {
			auto function = parse_function(it, tokens.end());
			if (std::get<0>(function)) {
				functions.push_back(std::move(std::get<0>(function)));
				it = std::get<1>(function);
			}
			else {
				break;
			}
		}
	}

	return nullptr;
}

std::tuple<std::unique_ptr<FunctionAST>, std::vector<Token>::iterator> parse_function(std::vector<Token>::iterator it, std::vector<Token>::iterator end) {
	it++; // Eat def
	auto proto = parse_prototype(it, end);
	if (!std::get<0>(proto)) return std::make_tuple(nullptr, it);

	auto expr = parse_expression(std::get<1>(proto), end);
	if (!std::get<0>(expr)) return std::make_tuple(nullptr, it);

	return std::make_tuple(std::make_unique<FunctionAST>(std::move(std::get<0>(proto)), std::move(std::get<0>(expr))), std::get<1>(expr));
}

std::tuple<std::unique_ptr<PrototypeAST>, std::vector<Token>::iterator> parse_prototype(std::vector<Token>::iterator it, std::vector<Token>::iterator end) {
	if (it != end && (*it).type != tok_identifier) {
		std::cout << "Expected function name in prototype" << '\n';
		return std::make_tuple(nullptr, it);
	}
	Token name = (*it++);

	if (it != end && (*it).literal == "(") it++;
	else {
		std::cout << "Expected '(' in prototype" << '\n';
		return std::make_tuple(nullptr, it);
	}

	std::vector<Token> args;
	while ((*it).type == tok_identifier) {
		args.push_back(*it++);
	}

	if (it != end && (*it).literal == ")") it++;
	else {
		std::cout << "Expected ')' in prototype" << '\n';
		return std::make_tuple(nullptr, it);
	}

	return std::make_tuple(std::make_unique<PrototypeAST>(name, std::move(args)), it);
}

std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_expression(std::vector<Token>::iterator it, std::vector<Token>::iterator end) {
	
}
