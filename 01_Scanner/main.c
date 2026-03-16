// main.c

#define extern_
#include "global.h"
#undef extern_

#include "token.h"

#include "scan.h"

static void init() {
    Line = 1;
    Putback = '\n';
}

char *tokstr[] = {"+", "-", "*", "/", "intlit"};

/*
 * @brief 扫描文件中的 token
 * @return 无
 */
static void scanfile() {
    struct token T;

    // 扫描文件中的 token 直到文件末尾
    while (scan(&T)) {
        printf("Token %s", tokstr[T.token]);

        // 打印整数 token 的值
        if (T.token == T_INTLIT) {
            printf(", value %d", T.intvalue);
        }
        printf("\n");
    }
}

/*
 * @brief 打印程序使用方法
 * @param prog 程序名称
 * @return 无
 */
static void usage(char *prog) {
  fprintf(stderr, "Usage: %s infile\n", prog);
  exit(1);
}

int main(int argc, char *argv[]) {
    // 检查命令行参数数量
    if (argc != 2) {
        usage(argv[0]);
    }

    // 初始化全局变量
    init();

    // 打开输入文件
    if ((Infile = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Can't open %s: ", argv[1]);
        exit(1);
    }

    // 扫描文件中的 token
    scanfile();

    // 退出程序
    exit(0);
}
