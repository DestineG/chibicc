// main.c

#define extern_
#include "global.h"
#undef extern_
#include "scan.h"
#include "ast.h"

static void init() {
    Line = 1;
    Putback = '\n';
}

static void usage(char *prog) {
    fprintf(stderr, "Usage: %s infile\n", prog);
    exit(1);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        usage(argv[0]);
    }
    init();
    struct ASTnode *n;

    // 打开输入文件
    if((Infile = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Can't open %s: ", argv[1]);
        exit(1);
    }

    // 解析表达式
    n = binexpr();

    // 解释 AST
    printf("%d\n", interpretAST(n));

    // 退出程序
    return 0;
}