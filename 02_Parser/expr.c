// expr.c

#include "global.h"
#include "ast.h"
#include "scan.h"

/**
 * @brief 解析整数字面量
 * @return 整数字面量的 AST 节点
 */
static struct ASTnode *primary(void) {
    struct ASTnode *n;

    switch(Token.token) {
    case T_INTLIT:
        n = mkastleaf(A_INTLIT, Token.intvalue);
        return n;
    default:
        fprintf(stderr, "primary: unexpected token %d\n", Token.token);
        exit(1);
    }
}

/**
 * @brief 解析算术运算符
 * @param tok 运算符 token
 * @return 对应的 AST 操作符
 */
int arithop(int tok) {
    switch(tok) {
        case T_PLUS: return A_ADD;
        case T_MINUS: return A_SUB;
        case T_STAR: return A_MUL;
        case T_SLASH: return A_DIV;
        default:
            fprintf(stderr, "arithop: unexpected token %d\n", tok);
            exit(1);
    }
}

/**
 * @brief 解析表达式
 * @return 表达式的 AST 节点
 */
struct ASTnode *binexpr(void) {
    struct ASTnode *n, *left, *right;
    int nodetype;

    // 扫描左操作数 token
    scan(&Token);
    left = primary();

    // 扫描运算符 token
    scan(&Token);
    if(Token.token == T_EOF) {
        return left;
    }
    nodetype = arithop(Token.token);

    // 扫描右操作数 token
    right = binexpr();

    // 构建 AST 节点
    n = mkastnode(nodetype, left, right, 0);

    return n;
}