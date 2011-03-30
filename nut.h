#ifndef NUTSHELL_MODULE_H
#define NUTSHELL_MODULE_H

#define nut_init(x) int __nut_init() { return x(); }
#define nut_exit(x) void __nut_exit() { x(); }

#define NUT_NAME(x) char __nut_name[] = x;

typedef int (*init_t)();
typedef void (*exit_t)();

int init_nuts();
void exit_nuts();

void *load_module(char *path);
void *get_symbol(void *ptr, char *sym);
void unload_module(void *ptr);

#endif
