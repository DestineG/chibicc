// 05_Statements/src/backend/gen.h

#ifndef GEN_H
#define GEN_H

#include "../midend/ast.h"

int translateAst(struct ASTnode *n, int reg, int parentASTop);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);

#endif
