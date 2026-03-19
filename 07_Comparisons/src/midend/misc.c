// 05_Statements/src/midend/misc.c

#include "../frontend/scan.h"
#include "misc.h"

/**
 * @brief 匹配指定的 token 类型并获取下一个 token
 * 
 * @param t 要匹配的 token 类型
 * @param what 用于错误提示的字符串
 */
void match(int t, char *what) {
  if (Token.token == t) {
    scan(&Token);
  } else {
    printf("%s expected on line %d\n", what, Line);
    exit(1);
  }
}

/**
 * @brief 匹配分号并获取下一个 token
 * 
 */
void semi(void) {
  match(T_SEMI, ";");
}

/**
 * @brief 匹配标识符并获取下一个 token
 * 
 */
void ident(void) {
  match(T_IDENT, "identifier");
}

/**
 * @brief 打印错误消息并退出程序
 * 
 * @param s 错误消息字符串
 */
void fatal(char *s) {
  fprintf(stderr, "%s on line %d\n", s, Line); exit(1);
}

/**
 * @brief 打印错误消息并退出程序
 * 
 * @param s1 错误消息字符串1
 * @param s2 错误消息字符串2
 */
void fatals(char *s1, char *s2) {
  fprintf(stderr, "%s:%s on line %d\n", s1, s2, Line);
  exit(1);
}

/**
 * @brief 打印错误消息并退出程序
 * 
 * @param s 错误消息字符串
 * @param d 整数参数
 */
void fatald(char *s, int d) {
  fprintf(stderr, "%s:%d on line %d\n", s, d, Line); exit(1);
}

/**
 * @brief 打印错误消息并退出程序
 * 
 * @param s 错误消息字符串
 * @param c 字符参数
 */
void fatalc(char *s, int c) {
  fprintf(stderr, "%s:%c on line %d\n", s, c, Line); exit(1);
}