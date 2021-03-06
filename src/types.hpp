#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>
#include <tuple>
#include <optional>
#include <map>
#include <memory>

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

// Abstract Syntax Tree
// --------------------
struct ExprAST {
	virtual ~ExprAST() {}
	virtual void print(int indent_level) = 0;
};

struct NumberExprAst : ExprAST {
	double val;
	NumberExprAst(double val) : val(val) {}
	void print(int indent_level);
};

struct VariableExprAST : ExprAST {
	std::string name;
	VariableExprAST(std::string name) : name(name) {}
	void print(int indent_level);
};

struct BinaryExprAst : ExprAST {
	Token op;
	std::unique_ptr<ExprAST> left;
	std::unique_ptr<ExprAST> right;
	BinaryExprAst(Token op, std::unique_ptr<ExprAST> left, std::unique_ptr<ExprAST> right) : op(op), left(std::move(left)), right(std::move(right)) {}
	void print(int indent_level);
};

struct CallExprAST : ExprAST {
	std::string callee;
	std::vector<std::unique_ptr<ExprAST>> args;
	CallExprAST(std::string callee, std::vector<std::unique_ptr<ExprAST>> args) : callee(callee), args(std::move(args)) {}
	void print(int indent_level);
};

struct PrototypeAST {
	Token name;
	std::vector<Token> args;
	PrototypeAST(Token name, std::vector<Token> args) : name(name), args(std::move(args)) {}

	const std::string &getName() const {return name.literal;}
};

struct FunctionAST {
	std::unique_ptr<PrototypeAST> prototype;
	std::unique_ptr<ExprAST> body;
	FunctionAST(std::unique_ptr<PrototypeAST> prototype, std::unique_ptr<ExprAST> body) : prototype(move(prototype)), body(move(body)) {}
};
void print(FunctionAST* fun, int indent_level = 0);

struct ProgramAST {
	std::vector<std::unique_ptr<FunctionAST>> functions;
	std::vector<std::unique_ptr<ExprAST>> expressions;
	ProgramAST(std::vector<std::unique_ptr<FunctionAST>> functions, std::vector<std::unique_ptr<ExprAST>> expressions) : functions(move(functions)), expressions(move(expressions)) {}
};
void print(ProgramAST* prg);

#endif
