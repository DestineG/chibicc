// 06_Variables/src/midend/symbol.h

#ifndef SYMBOL_H
#define SYMBOL_H

#define NSYMBOLS 1024

struct symtable {
  char *name;
};

#ifndef extern_
    #define extern_ extern
#endif

extern_ struct symtable Gsym[NSYMBOLS];

void show_Gsym(void);
int findglob(char *s);
int addglob(char *name);

#endif