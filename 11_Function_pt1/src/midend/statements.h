// 05_Statements/src/midend/statements.h

#ifndef STATEMENTS_H
#define STATEMENTS_H

struct ASTnode *single_statement(void);
struct ASTnode *compound_statement(void);
struct ASTnode *function_declaration(void);

#endif