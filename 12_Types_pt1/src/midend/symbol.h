// 06_Variables/src/midend/symbol.h

#ifndef SYMBOL_H
#define SYMBOL_H

#define NSYMBOLS 1024

// Primitive type; ast type
enum {
    P_NONE, P_VOID, P_CHAR, P_INT
};

// Structural types; symbol: func | var
enum {
    S_VARIABLE, S_FUNCTION
};

struct symtable {
  char *name;
  int type;
  int stype;
};

#ifndef extern_
    #define extern_ extern
#endif

extern_ struct symtable Gsym[NSYMBOLS];

void show_Gsym(void);
int findglob(char *s);
int addglob(char *name, int type, int stype);

#endif