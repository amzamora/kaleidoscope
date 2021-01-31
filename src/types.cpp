#include <iostream>

#include "types.hpp"

void print_indent_level(int indent_level) {
    for (unsigned int i = 0; i < indent_level; i++) {
        std::cout << "    ";
    }
}

void print(ProgramAST* prg) {
    std::cout << "program" << '\n';

    for (unsigned int i = 0; i < prg->functions.size(); i++) {
        print(prg->functions[i].get(), 1);
    }

    for (unsigned int i = 0; i < prg->expressions.size(); i++) {
        prg->expressions[i].get()->print(1);
    }
}

void print(FunctionAST* fun, int indent_level) {
    print_indent_level(indent_level);
    std::cout <<  fun->prototype->name.literal << "(";
    for (unsigned int i = 0; i < fun->prototype->args.size(); i++) {
        std::cout << fun->prototype->args[i].literal;
        if (i != fun->prototype->args.size() - 1) std::cout << " ";
    }
    std::cout << ")\n";
    fun->body->print(indent_level + 1);
}

void VariableExprAST::print(int indent_level) {
    print_indent_level(indent_level);
    std::cout << this->name << "\n";
}

void NumberExprAst::print(int indent_level) {
    print_indent_level(indent_level);
    std::cout << this->val << '\n';
}

void BinaryExprAst::print(int indent_level) {
    print_indent_level(indent_level);
    std::cout << this->op.literal << '\n';
    this->left->print(indent_level + 1);
    this->right->print(indent_level + 1);
}

void CallExprAST::print(int indent_level) {
    print_indent_level(indent_level);
    std::cout << this->callee << '\n';
    for (unsigned int i = 0; i < this->args.size(); i++) {
        this->args[i]->print(indent_level + 1);
    }
}
