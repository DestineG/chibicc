// 04_Assembly/src/midend/expr.c

#define extern_
#include "../frontend/scan.h"
#undef extern_

#include "ast.h"

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

static int OpPrec[] = { 0, 10, 10, 20, 20, 0};

/**
 * @brief 获取运算符的优先级
 * @param tok_type 运算符 token 类型
 * @return 运算符的优先级
 */
static int op_precedence(int tok_type) {
    int prec = OpPrec[tok_type];
    if(prec == 0) {
        fprintf(stderr, "op_precedence: unknown operator %d\n", tok_type);
        exit(1);
    }
    return prec;
}

/**
 * @brief 解析表达式
 * @param ptp 父表达式的优先级
 * @return 表达式的 AST 节点
 */
struct ASTnode *binexpr(int ptp) {
    struct ASTnode *left, *right;
    int nodetype;

    // 扫描左操作数 token
    scan(&Token);
    left = primary();

    // 扫描运算符 token
    scan(&Token);
    if(Token.token == T_EOF) {
        return left;
    }

    // 若运算符的优先级大于父表达式的优先级，则递归解析右操作数
    while(op_precedence(Token.token) > ptp) {
        nodetype = arithop(Token.token);
        right = binexpr(op_precedence(Token.token));
        left = mkastnode(nodetype, left, right, 0);
        if(Token.token == T_EOF) {
            break;
        }
    }

    return left;
}