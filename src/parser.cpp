#include <iostream>

#include "types.hpp"

// Prototypes
// ----------
std::tuple<std::unique_ptr<FunctionAST>, std::vector<Token>::iterator> parse_function(std::vector<Token>::iterator it, std::vector<Token>::iterator end);
std::tuple<std::unique_ptr<PrototypeAST>, std::vector<Token>::iterator> parse_prototype(std::vector<Token>::iterator it, std::vector<Token>::iterator end);
std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_expression(std::vector<Token>::iterator it, std::vector<Token>::iterator end);
std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_bin_op_RHS(std::vector<Token>::iterator it, std::vector<Token>::iterator end, int expr_precedence,  std::unique_ptr<ExprAST> LHS);
int get_token_precedence(std::vector<Token>::iterator it);
std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_primary(std::vector<Token>::iterator it, std::vector<Token>::iterator end);
std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_identifier_expr(std::vector<Token>::iterator it, std::vector<Token>::iterator end);
std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_number_expr(std::vector<Token>::iterator it, std::vector<Token>::iterator end);
std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_paren_epxr(std::vector<Token>::iterator it, std::vector<Token>::iterator end);


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
	auto LHS = parse_primary(it, end);
	if (!std::get<0>(LHS)) return std::make_tuple(nullptr, it);
	return parse_bin_op_RHS(it, end, 0, std::move(std::get<0>(LHS)));
}

std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_bin_op_RHS(std::vector<Token>::iterator it, std::vector<Token>::iterator end, int expr_precedence,  std::unique_ptr<ExprAST> LHS) {
	while (true) {
		int precedence = get_token_precedence(it);
		if (precedence < expr_precedence) return make_tuple(std::move(LHS), it);

		Token op = *it++;

		auto RHS = parse_primary(it, end);
		if (!std::get<0>(RHS)) return std::make_tuple(nullptr, it);

		int next_precedence = get_token_precedence(it);
		if (precedence < next_precedence) {
			RHS = parse_bin_op_RHS(it, end, precedence + 1, std::get<0>(std::move(RHS)));
			if (!std::get<0>(RHS)) return std::make_tuple(nullptr, it);
		}

		LHS = std::make_unique<BinaryExprAst>(op, std::move(LHS), std::move(std::get<0>(RHS)));
	}
}

int get_token_precedence(std::vector<Token>::iterator it) {
	static std::map<char, int> BinopPrecedence;
	BinopPrecedence['<'] = 10;
	BinopPrecedence['+'] = 20;
	BinopPrecedence['-'] = 20;
	BinopPrecedence['*'] = 40;

	if (!isascii((*it).literal.c_str()[0])) return -1;

	int precedence = BinopPrecedence[(*it).literal.c_str()[0]];
	if (precedence <= 0) return -1;
	return precedence;
}

std::tuple<std::unique_ptr<ExprAST>, std::vector<Token>::iterator> parse_primary(std::vector<Token>::iterator it, std::vector<Token>::iterator end) {
	if ((*it).type == tok_identifier) return parse_identifier_expr(it, end);
	if ((*it).type == tok_number)     return parse_number_expr(it, end);
	if ((*it).literal == "(")         return parse_paren_epxr(it, end);
	else {
		std::cout << "Unknown token when expecting expression" << '\n';
		return std::make_tuple(nullptr, it);
	}
}
