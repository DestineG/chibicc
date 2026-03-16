// scan.c

#include <ctype.h>
#include <string.h>
#include "global.h"
#include "token.h"

/*
 * @brief 查找字符串 s 中字符 c 首次出现的位置
 * @param s 要搜索的字符串
 * @param c 要搜索的字符
 * @return 字符c首次出现的位置(0从开始), 如果字符c未找到, 则返回-1
 */
static int chrpos(char *s, int c) {
    char *p;
    p = strchr(s, c);
    return p ? p - s : -1;
}

/*
 * @brief 读取 Infile 文件句柄中的下一个字符
 * * 若 Putback 存在, 则先返回 Putback 中的字符
 * @return 读取到的字符
 */
static int next(void) {
    int c;

    // 若 Putback 存在, 则先返回 Putback 中的字符
    if (Putback) {
        c = Putback;
        Putback = 0;
        return c;
    }

    // 从 Infile 文件句柄中读取下一个字符
    c = fgetc(Infile);

    // 若读取到的字符为换行符, 则增加 Line 计数器
    if ('\n' == c) {
        Line++;
    }

    return c;
}

/*
 * @brief 将字符 c 放回 Putback 中
 * @param c 要放回的字符
 */
static void putback(int c) {
    Putback = c;
}

/*
 * @brief 跳过 Infile 文件句柄中的空白字符 | 制表符 | 换行符 | 回车符 | 换页符
 * @return 第一个非空白字符
 */
static int skip(void) {
    int c;

    do {
        c = next();
    } while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c);

    return c;
}

/*
 * @brief 扫描整数 token
 * @param c 第一个字符
 * @return 扫描到的整数
 */
static int scanint(int c) {
    int k, val = 0;

    // 扫描整数部分
    while((k = chrpos("0123456789", c)) >= 0) {
        val = val * 10 + k;
        c = next();
    }

    // 将最后一个读取的非数字字符放回 Putback 中
    // 以便后续扫描其他类型的 token
    putback(c);

    return val;
}

/*
 * @brief 扫描 token
 * @param t 指向 token 结构体的指针
 * @return 1 成功扫描到 token, 0 到达文件末尾
 */
int scan(struct token *t) {
    int c;

    // 跳过空白字符 | 制表符 | 换行符 | 回车符 | 换页符
    // 取到第一个非空白字符
    c= skip();

    // 处理特殊字符
    switch(c) {
    case EOF:
        return 0;
    case '+':
        t->token = T_PLUS;
        break;
    case '-':
        t->token = T_MINUS;
        break;
    case '*':
        t->token = T_STAR;
        break;
    case '/':
        t->token = T_SLASH;
        break;
    default:
        if(isdigit(c)) {
            t->token = T_INTLIT;
            t->intvalue = scanint(c);
            break;
        }
        printf("Unrecognised character %c at line %d\n", c, Line);
        exit(1);
    }

    return 1;
}