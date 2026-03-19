// 04_Assembly/src/midend/expr.c

#include <stdio.h>
#define extern_
#include "../frontend/scan.h"
#undef extern_

#include "ast.h"
#include "symbol.h"
#include "misc.h"
#include "expc.h"

/**
 * @brief 解析整数字面量
 * @return 整数字面量的 AST 节点
 */
static struct ASTnode *primary(void) {
    struct ASTnode *n;
    int id;

    switch(Token.token) {
        // 解析整数字面量
        case T_INTLIT:
            n = mkastleaf(A_INTLIT, Token.intvalue);
            scan(&Token);
            return n;
        // 解析标识符
        case T_IDENT:
            id = findglob(Text);
            if(id == -1) {
                fatals("Unknown variable", Text);
            }
            // 打印调试信息
            if(*Text == 'f' || *Text == 'j') {
                printf("primary: text %s id %d\n", Text, id);
            }
            n = mkastleaf(A_IDENT, id);
            scan(&Token);
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
int arithop(int tokentype) {
    // NOTE: 此处要求 token 和 ast 的操作符类型索引一一对应
    if(tokentype > T_EOF && tokentype < T_INTLIT) {
        return tokentype;
    }
    fatald("Syntax error, token", tokentype);
    return -1;
}

static int OpPrec[] = {
    // T_EOF
    0,
    // T_PLUS, T_MINUS, T_STAR, T_SLASH
    10, 10, 20, 20,
    // T_EQ, T_NE
    30, 30,
    // T_LT, T_GT, T_LE, T_GE
    40, 40, 40, 40
};

/**
 * @brief 获取运算符的优先级
 * @param tok_type 运算符 token 类型
 * @return 运算符的优先级
 */
static int op_precedence(int tok_type) {
    int prec = OpPrec[tok_type];
    return prec;
}

/**
 * @brief 解析表达式
 * @param ptp 父表达式的优先级
 * @return 表达式的 AST 节点
 */
struct ASTnode *binexpr(int ptp) {
    struct ASTnode *left, *right;
    int tokentype;

    // 获取左操作数节点，同时扫描下一个 token
    left = primary();

    // 如果是分号或文件末尾，说明表达式结束
    tokentype = Token.token;
    if(tokentype == T_SEMI || tokentype == T_EOF) {
        return left;
    }

    // 若运算符的优先级大于父表达式的优先级，则递归解析右操作数
    while(op_precedence(tokentype) > ptp) {
        // 扫描下一个 token (应该是右操作数)
        scan(&Token);

        // 递归解析右操作数
        right = binexpr(OpPrec[tokentype]);

        // 将左操作数、运算符和右操作数合并为 AST 节点
        left = mkastnode(arithop(tokentype), left, right, 0);

        // 更新当前运算符 token
        tokentype = Token.token;
        if(tokentype == T_SEMI || tokentype == T_EOF) {
            break;
        }
    }

    return left;
}