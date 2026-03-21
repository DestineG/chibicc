// ast.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "misc.h"

/**
 * @brief 创建 AST 节点
 * @param op 操作符
 * @param type 类型
 * @param left 左子节点
 * @param right 右子节点
 * @param intvalue 整数值
 * @return 新创建的 AST 节点
 */
struct ASTnode *mkastnode(
    int op, int type,
    struct ASTnode *left,
    struct ASTnode *mid,
    struct ASTnode *right,
    int intvalue
) {
    struct ASTnode *n = malloc(sizeof(struct ASTnode));
    if(n == NULL) {
        fatal("Unable to malloc in mkastnode()");
    }
    n->op = op;
    n->type = type;
    n->left = left;
    n->mid = mid;
    n->right = right;
    n->v.intvalue = intvalue;
    return n;
}

/**
 * @brief 创建 AST 叶子节点
 * @param op 操作符
 * @param type 类型
 * @param intvalue 整数值
 * @return 新创建的 AST 叶子节点
 */
struct ASTnode *mkastleaf(int op, int type, int intvalue) {
  return (mkastnode(op, type, NULL, NULL, NULL, intvalue));
}

/**
 * @brief 创建 AST 一元节点
 * @param op 操作符
 * @param type 类型
 * @param left 左子节点
 * @param intvalue 整数值
 * @return 新创建的 AST 一元节点
 */
struct ASTnode *mkastunary(int op, int type, struct ASTnode *left, int intvalue) {
    return mkastnode(op, type, left, NULL, NULL, intvalue);
}

static char *ASTop[] = { "+", "-", "*", "/" };

/**
 * @brief 解释 AST 节点
 * @param n AST 节点
 * @return 节点的值
 */
int interpretAST(struct ASTnode *n) {
    int leftval, rightval;

    if(n->left) {
        leftval = interpretAST(n->left);
    }

    if(n->right) {
        rightval = interpretAST(n->right);
    }

    if(n->op == A_INTLIT) {
        printf("int %d\n", n->v.intvalue);
    }
    else {
        printf("%d %s %d\n", leftval, ASTop[n->op], rightval);
    }
        
    switch(n->op) {
    case A_ADD:
        return leftval + rightval;
    case A_SUB:
        return leftval - rightval;
    case A_MUL:
        return leftval * rightval;
    case A_DIV:
        return leftval / rightval;
    case A_INTLIT:
        return n->v.intvalue;
    default:
        fprintf(stderr, "Unknown AST operator %d\n", n->op);
        exit(1);
    }
}

// 简单的队列节点结构

struct QNode {
    struct ASTnode *ast_node;
    int x;          // 节点的横向位置
    int level;      // 所在的层级
    struct QNode *next;
};

// 简单的队列操作省略，直接在函数内实现
void dumpAST_level_tree(struct ASTnode *root) {
    if (!root) return;

    struct QNode *head = malloc(sizeof(struct QNode));
    head->ast_node = root;
    head->level = 0;
    head->x = 40; // 假设根节点在中间
    head->next = NULL;
    struct QNode *tail = head;

    int current_level = -1;
    char line[100]; // 用于缓冲一层的显示

    while (head) {
        struct QNode *curr = head;
        if (curr->level > current_level) {
            if (current_level != -1) printf("\n\n"); // 换层
            current_level = curr->level;
            memset(line, ' ', 99);
            line[99] = '\0';
            printf("Level %d:", current_level);
        }

        // 将节点内容填入缓冲区
        char buf[16];
        if (curr->ast_node->op == A_INTLIT) 
            sprintf(buf, "%d", curr->ast_node->v.intvalue);
        else 
            sprintf(buf, "[%s]", ASTop[curr->ast_node->op]);
        
        memcpy(&line[curr->x], buf, strlen(buf));

        // 子节点入队，并计算偏移量
        int offset = 10 / (curr->level + 1); // 随着层级加深，间距缩小
        if (curr->ast_node->left) {
            struct QNode *l = malloc(sizeof(struct QNode));
            l->ast_node = curr->ast_node->left;
            l->level = curr->level + 1;
            l->x = curr->x - offset;
            l->next = NULL;
            tail->next = l;
            tail = l;
        }
        if (curr->ast_node->right) {
            struct QNode *r = malloc(sizeof(struct QNode));
            r->ast_node = curr->ast_node->right;
            r->level = curr->level + 1;
            r->x = curr->x + offset;
            r->next = NULL;
            tail->next = r;
            tail = r;
        }

        // 打印这一层的所有节点（如果下一个节点换层了）
        if (!head->next || head->next->level > current_level) {
            printf("%s", line);
        }

        head = head->next;
        free(curr);
    }
    printf("\n");
}