#ifndef _MONITORFILE_H
#define _MONITORFILE_h

void sys_err(char *str);

void demon();

void dirwalk(List *lp, char *path, void (*fun)(List *lp, char *name));

void fize(List *lp, char *name);

void monitorfile();

#endif
