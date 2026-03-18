// 05_Statements/src/midend/statements.c

#include "../frontend/scan.h"
#include "ast.h"
#include "expc.h"
#include "misc.h"
#include "../backend/gen.h"
#include "../backend/cgen.h"
#include "symbol.h"

/**
 * @brief 确认并消费 print 关键字，然后解析后续表达式并生成汇编代码
 * 
 */
void print_statement(void) {
  struct ASTnode *tree;
  int reg;

  // 确认并消费 print 关键字
  match(T_PRINT, "print");

  // 解析后续表达式
  tree = binexpr(0);
  // 翻译并生成汇编代码
  reg = translateAst(tree, -1);
  // 生成打印整数的汇编代码
  genprintint(reg);
  // 释放寄存器
  genfreeregs();

  // 消费分号; 如果是 EOF 则结束
  semi();
}

/**
 * @brief 确认并消费 int 关键字，然后确认并消费变量标识符，最后添加全局符号到符号表并生成汇编代码
 * 
 */
void var_declaration(void) {
  // 确认并消费 int 关键字
  match(T_INT, "int");

  // 确认并消费变量标识符
  ident();

  // 添加全局符号到符号表
  addglob(Text);

  // 生成全局符号的汇编代码
  cgglobsym(Text);

  // 确认并消费分号;
  semi();
}

void assignment_statement(void) {
  struct ASTnode *left, *right, *tree;
  int id;

  // 确认并消费变量标识符
  ident();

  // 查找变量标识符在符号表中的索引
  if((id = findglob(Text)) == -1) {
    fatals("Unknown variable", Text);
  }

  // 生成右值 AST 节点
  right = mkastleaf(A_LVIDENT, id);

  match(T_EQUALS, "=");

  // 解析后续表达式并生成汇编代码
  left = binexpr(0);

  // 生成赋值语句的 AST 节点
  tree = mkastnode(A_ASSIGN, left, right, 0);

  // 翻译并生成汇编代码
  translateAst(tree, -1);
  genfreeregs();

  // 确认并消费分号;
  semi();
}

void statements(void) {
  while (1) {
    switch (Token.token) {
    case T_PRINT:
      print_statement();
      break;
    case T_INT:
      var_declaration();
      break;
    case T_IDENT:
      assignment_statement();
      break;
    case T_EOF:
      return;
    default:
      fatald("Syntax error, token", Token.token);
    }
  }
}