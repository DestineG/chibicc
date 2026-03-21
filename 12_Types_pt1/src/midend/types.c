// src/midend/types.c

#include "ast.h"
#include "symbol.h"

/**
 * @brief 检查两个类型是否兼容
 * @param left 左边的类型
 * @param right 右边的类型
 * @param onlyright 仅检查右边类型(禁止扩展右端)
 * @return 类型是否兼容
 */
int type_compatible(int *left, int *right, int onlyright) {
    // 如果任一类型是 void，则不兼容
    if((*left == P_VOID) || (*right == P_VOID)) {
        return 0;
    }

    // 如果类型完全相同，则兼容
    if(*left == *right) {
        *left = *right = 0;
        return 1;
    }

    // 左边类型需要拓宽
    if(*left == P_CHAR && *right == P_INT) {
        *left = A_WIDEN;
        *right = 0;
        return 1;
    }

    // 右边类型需要拓宽
    if(*left == P_INT && *right == P_CHAR) {
        if (onlyright) return 0;
        *left = 0;
        *right = A_WIDEN;
        return 1;
    }

    // 类型不兼容
    *left = *right = 0;
    return 0;
}