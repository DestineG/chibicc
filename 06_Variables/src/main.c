// 04_Assembly/main.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "frontend/scan.h"
#include "midend/statements.h"
#include "backend/cgen.h"
#include "backend/gen.h"

static void init() {
    Line = 1;
    Putback = '\n';
}

static void usage(char *prog) {
    fprintf(stderr, "Usage: %s infile\n", prog);
    exit(1);
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        usage(argv[0]);
    }
    init();

    // 打开输入文件
    if((Infile = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Can't open %s: ", argv[1]);
        exit(1);
    }

    // 打开输出文件
    if((Outfile = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "Can't open out.s: ");
        exit(1);
    }

    // 生成汇编前导
    genpreamble();
    // 扫描第一个 token
    scan(&Token);
    // 生成语句
    statements();
    // 生成汇编后导
    genpostamble();

    // 关闭输出文件
    fclose(Outfile);

    // 退出程序
    return 0;
}