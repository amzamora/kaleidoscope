#include "types.hpp"
using namespace std;

// Prototypes
// ----------
tuple<optional<Token>, string::iterator> get_token(string::iterator it, string::iterator end);
string::iterator advance_until_new_line(string::iterator it, string::iterator end);
tuple<optional<Token>, string::iterator> get_string(string::iterator it, string::iterator end);
tuple<optional<Token>, string::iterator> get_identifier(string::iterator it, string::iterator end);
tuple<optional<Token>, string::iterator> get_integer(string::iterator it, string::iterator end);

// Implementations
// ---------------
vector<Token> lex(string file) {
	vector<Token> tokens;

	auto it = file.begin();
	while (it != file.end()) {
		auto result = get_token(it, file.end());
		if (get<0>(result).has_value()) tokens.push_back(get<0>(result).value());
		it = get<1>(result);
	}

	return tokens;
}

tuple<optional<Token>, string::iterator> get_token(string::iterator it, string::iterator end) {
	if (it == end)  return make_tuple(nullopt, it);
	if (*it == '#') return get_token(advance_until_new_line(it, end), end);
	if (*it == ' ' || *it == '\t' || *it == '\n') return get_token(it + 1, end);
	if (isdigit(*it)) return get_integer(it, end);
	if (isalnum(*it)) return get_identifier(it, end);
	else              return make_tuple(Token(tok_char, std::string(1, *it)), it + 1);
}

string::iterator advance_until_new_line(string::iterator it, string::iterator end) {
	while (!(it == end || *it == '\n')) it++;
	if (*it == '\n') return it + 1;
	else             return it;
}

tuple<optional<Token>, string::iterator> get_identifier(string::iterator it, string::iterator end) {
	string lit = "";
	while (!(it == end || *it == '\n' || !isalnum(*it))) {
		lit += *it;
		it++;
	}
	if (lit == "def")    return make_tuple(Token(tok_def, lit), it);
	if (lit == "extern") return make_tuple(Token(tok_extern, lit), it);
	else                 return make_tuple(Token(tok_identifier, lit), it);
}

#include <iostream>
tuple<optional<Token>, string::iterator> get_integer(string::iterator it, string::iterator end) {
	string lit = "";
	while (it != end && (isdigit(*it) || *it == '.'))  {
		lit += *it;
		it++;
	}
	double value = strtod(lit.c_str(), nullptr);
	return make_tuple(Token(tok_number, lit, value), it);
}
