// 05_Statements/src/midend/misc.c

#include "../include/global.h"
#include "../frontend/scan.h"
#include "misc.h"

void match(int t, char *what) {
  if (Token.token == t) {
    scan(&Token);
  } else {
    printf("%s expected on line %d\n", what, Line);
    exit(1);
  }
}

// Match a semicon and fetch the next token
void semi(void) {
  match(T_SEMI, ";");
}