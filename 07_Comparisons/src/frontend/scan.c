// scan.c

#include <ctype.h>
#include <string.h>

#include "../midend/misc.h"
#include "scan.h"

int Line;
int Putback;
FILE *Infile;
struct token Token;
char Text[TEXTLEN + 1];

/**
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

/**
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

/**
 * @brief 将字符 c 放回 Putback 中
 * @param c 要放回的字符
 */
static void putback(int c) {
    Putback = c;
}

/**
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

/**
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

/**
 * @brief 扫描标识符 token
 * @param c 第一个字符
 * @param buf 用于存储标识符的字符数组
 * @param lim buf 数组的大小
 * @return 扫描到的标识符的长度
 */
static int scanident(int c, char *buf, int lim) {
    int i = 0;

    // 扫描标识符部分
    while(isalpha(c) || isdigit(c) || c == '_') {
        // 检查标识符长度是否超过限制
        if(i == lim - 1) {
            printf("Identifier too long at line %d\n", Line);
            exit(1);
        }
        else if (i < lim - 1) {
            buf[i++] = c;
        }
        c = next();
    }

    // 将最后一个读取的非字母数字字符放回 Putback 中
    putback(c);

    // 结束标识符字符串
    buf[i] = '\0';

    return i;
}

// TODO: 实现关键字识别
static int keyword(char *s) {
    switch(*s) {
        case 'p':
            if (strcmp(s, "print") == 0) return T_PRINT;
            break;
        case 'i':
            if (strcmp(s, "int") == 0) return T_INT;
            break;
    }
    return 0;
}

/**
 * @brief 扫描 token
 * @param t 指向 token 结构体的指针
 * @return 1 成功扫描到 token, 0 到达文件末尾
 */
int scan(struct token *t) {
    int c, tokentype;

    // 跳过空白字符 | 制表符 | 换行符 | 回车符 | 换页符
    // 取到第一个非空白字符
    c= skip();

    // 处理特殊字符
    switch(c) {
    case EOF:
        t->token = T_EOF;
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
    case ';':
        t->token = T_SEMI;
        break;
    case '=':
        if((c = next()) == '=') {
            t->token = T_EQ;
        }
        else {
            putback(c);
            t->token = T_ASSIGN;
        }
        break;
    case '!':
        if((c = next()) == '=') {
            t->token = T_NE;
        }
        else {
            fatalc("Unrecognised chatacter", c);
        }
        break;
    case '<':
        if((c = next()) == '=') {
            t->token = T_LE;
        }
        else {
            putback(c);
            t->token = T_LT;
        }
        break;
    case '>':
        if((c = next()) == '=') {
            t->token = T_GE;
        }
        else {
            putback(c);
            t->token = T_GT;
        }
        break;
    default:
        // 处理数字
        if(isdigit(c)) {
            t->token = T_INTLIT;
            t->intvalue = scanint(c);
            break;
        }
        // 处理标识符
        else if(isalpha(c) || c == '_') {
            scanident(c, Text, TEXTLEN);
            if((tokentype = keyword(Text))) {
                t->token = tokentype;
                break;
            }
            // 未注册 keywords 标记为 identifier
            t->token = T_IDENT;
            break;
        }
        // 处理其他字符
        fatalc("Unrecognised character", c);
    }

    return 1;
}