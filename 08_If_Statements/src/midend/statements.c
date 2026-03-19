// 05_Statements/src/midend/statements.c

#include "../frontend/scan.h"
#include "ast.h"
#include "expc.h"
#include "misc.h"
#include "../backend/cgen.h"
#include "symbol.h"
#include "statements.h"

/**
 * @brief 生成 print 语句的 AST 节点
 * 
 * @return struct ASTnode* 生成的 print 语句的 AST 节点
 */
static struct ASTnode * print_statement(void) {
  struct ASTnode *tree;

  // 确认并消费 print 关键字
  match(T_PRINT, "print");

  // 构建 print 表达式的 AST
  tree = binexpr(0);

  // 构建 print AST
  tree = mkastunary(A_PRINT, tree, 0);
  // 消费分号; 如果是 EOF 则结束
  semi();
  return tree;
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

struct ASTnode * assignment_statement(void) {
  struct ASTnode *left, *right, *tree;
  int id;

  // 确认并消费变量标识符
  ident();

  // 查找变量标识符在符号表中的索引
  if((id = findglob(Text)) == -1) {
    fatals("Assignment unknown variable", Text);
  }

  // 生成左值 AST
  right = mkastleaf(A_LVIDENT, id);

  match(T_ASSIGN, "=");

  // 生成表达式 AST
  left = binexpr(0);

  // 生成赋值 AST
  tree = mkastnode(A_ASSIGN, left, NULL, right, 0);

  // 确认并消费分号;
  semi();

  return tree;
}

struct ASTnode *if_statement(void) {
  struct ASTnode *cond, *true, *false;

  // 消费 if (
  match(T_IF, "if");
  lparen();

  // 生成条件表达式 AST
  cond = binexpr(0);

  // 限制条件表达式为比较运算符
  if (cond->op < A_EQ || cond->op > A_GE)
    fatal("Bad comparison operator");

    // 消费 )
  rparen();

  // 消费 if 的 then 代码块
  true = compound_statement();

  // 消费 else 代码块
  if (Token.token == T_ELSE) {
    match(T_ELSE, "else");
    false = compound_statement();
  } else {
    false = NULL;
  }

  // 生成 if 语句的 AST
  return mkastnode(A_IF, cond, true, false, 0);
}

struct ASTnode *compound_statement(void) {
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  // 消费左花括号 {
  lbrace();

  // 构建复合语句的 AST
  while (1) {
    switch (Token.token) {
    case T_PRINT:
      tree = print_statement();
      break;
    case T_INT:
      // NOTE: 目前的变量声明仍然是实时声明，并未合并到 AST 中
      var_declaration();
      tree = NULL;
      break;
    case T_IDENT:
      tree = assignment_statement();
      break;
    case T_IF:
      tree = if_statement();
      break;
    // 消费右花括号 }
    case T_RBRACE:
      rbrace();
      return left;
    default:
      fatald("Syntax error, token", Token.token);
    }
    // 如果 tree 为 NULL，则跳过
    if(tree) {
      // 如果 left 为 NULL，则将 tree 赋值给 left
      if(left == NULL) {
        left = tree;
      }
      // 如果 left不为 NULL，则将 tree 与 left 合并
      // NOTE: 由于 汇编生成代码的遍历顺序是从左到右，所以新的语句 tree 会被放在 left 的右边
      else {
        left = mkastnode(A_GLUE, left, NULL, tree, 0);
      }
    }
  }
}