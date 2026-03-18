// 05_Statements/src/midend/statements.c

#include "../frontend/scan.h"
#include "misc.h"
#include "expc.h"
#include "../backend/gen.h"

void statements(void) {
  struct ASTnode *tree;
  int reg;

  scan(&Token);
  while (1) {
    // 消费 print
    match(T_PRINT, "print");

    // 解析后续表达式并生成汇编代码
    tree = binexpr(0);
    reg = genAST(tree);
    genprintint(reg);
    genfreeregs();

    // 消费分号; 如果是 EOF 则结束
    semi();
    if (Token.token == T_EOF)
      return;
  }
}