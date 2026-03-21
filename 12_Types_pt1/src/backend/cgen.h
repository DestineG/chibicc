// cgen.h

#ifndef CG_H
#define CG_H

#include <stdio.h>

#ifndef extern_
    #define extern_ extern
#endif

extern_ FILE *Outfile;

void show_regStatus();
void initreg(void);
void free_reg(int reg);
void cgpreamble(void);
void cgpostamble(void);

int cgloadint(int val);
int cgloadglob(int id);
int cgstorglob(int r, int id);
void cgglobsym(int id);

int cgadd(int reg1, int reg2);
int cgsub(int reg1, int reg2);
int cgmul(int reg1, int reg2);
int cgdiv(int reg1, int reg2);

void cgprintint(int reg);

void cglabel(int l);
void cgjump(int l);
int cgcompare_and_set(int ASTop, int reg1, int reg2);
int cgcompare_and_jump(int ASTop, int reg1, int reg2, int label);

void cgfuncpreamble(char *name);
void cgfuncpostamble();

int cgwiden(int r, int oldtype, int newtype);

#endif