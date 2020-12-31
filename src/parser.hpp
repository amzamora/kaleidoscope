#ifndef PARSER_HPP
#define PARSER_HPP

std::unique_ptr<ProgramAST> parse(std::vector<Token> tokens);

#endif
