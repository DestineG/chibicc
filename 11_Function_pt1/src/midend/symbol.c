// 06_Variables/src/midend/symbol.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define extern_
#include "symbol.h"
#undef extern_

static int Globs = 0;

void show_Gsym(void) {
    if(Globs == 0) {
        printf("No globals\n");
        return;
    }
    for(int i = 0; i < Globs; i++) {
        printf("Gsym[%d].name: %s\n", i, Gsym[i].name);
    }
}

/**
 * @brief 查找全局变量
 * 
 * @param s 全局变量的名称
 * @return int 全局变量的索引
 */
int findglob(char *s) {
    int i;
    for(i = 0; i < Globs; i++) {
        if(*s == *Gsym[i].name && strcmp(s, Gsym[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief 新建一个全局变量
 * 
 * @return int 全局变量的索引
 */
static int newglob(void) {
    int p;

    if((p = Globs++) >= NSYMBOLS) {
        fprintf(stderr, "Too many globals\n");
        exit(1);
    }
    return p;
}

/**
 * @brief 添加一个全局变量
 * 
 * @param name 全局变量的名称
 * @return int 全局变量的索引
 */
int addglob(char *name) {
    int y;

    if((y = findglob(name)) != -1) {
        return y;
    }

    y = newglob();
    Gsym[y].name = strdup(name);
    return y;
}