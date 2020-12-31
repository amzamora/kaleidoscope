#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>
#include <tuple>
#include <optional>

// Token
// -----
enum TokenType {
	// commands
	tok_def,
	tok_extern,

	// primary
	tok_identifier,
	tok_number,
	tok_char,
};

struct Token {
	TokenType type;
	std::string literal;
	double value;

	Token() {}
	Token(TokenType type, std::string literal = "") : type(type), literal(literal) {}
	Token(TokenType type, std::string literal, double value) : type(type), literal(literal), value(value) {}
};

#endif
