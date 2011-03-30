#include <nut.h>
#include <util.h>

#include <sys/types.h>
#include <dlfcn.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct nut {
	char *name;
	void *module;
	init_t init;
	exit_t exit;
	struct nut *next;
};

struct nut *nuts = NULL;

int verify_nut(void *mod)
{
	init_t init;
	exit_t exit;
	char *name;
	init = (init_t)get_symbol(mod, "__nut_init");
	if (!init)
		return -1;
	exit = (exit_t)get_symbol(mod, "__nut_exit");
	if (!exit)
		return -1;
	name = (char *)get_symbol(mod, "__nut_name");
	if (!name)
		return -1;
	struct nut *tmp = nuts;
	while (tmp)
		if (strcmp(tmp->name, name) == 0)
			return -1;
	struct nut *n = malloc(sizeof(struct nut));
	if (!n) {
		printf("nut failed to open: %s\n", name);
		unload_module(mod);
		return -1;
	}
	n->name = malloc(strlen(name) + 1);
	if (!n->name) {
		printf("nut failed to open: %s\n", name);
		unload_module(mod);
		free(n);
		return -1;
	}
	strcpy(n->name, name);
	n->module = mod;
	n->next = nuts;
	n->init = init;
	n->exit = exit;
	nuts = n;
	n->init();
	printf("nut loaded: %s\n", n->name);
	return 0;
}

int init_nuts()
{
	DIR *dir;
	void *mod;
	struct dirent *dp;
	char *dir_path = "./";
	char buffer[1024];
	dir = opendir(dir_path);
	if (!dir) {
		printf("failed to read directory\n");
		return -1;
	}
	while ((dp = readdir(dir)) != NULL) {
		if (strrstr(dp->d_name, ".nut") == NULL)
			continue;
		strcpy(buffer, dir_path);
		strcat(buffer, dp->d_name);
		mod = load_module(buffer);
		if (!mod) {
			printf("nut failed to open: %s\n", dp->d_name);
			continue;
		}
		if (verify_nut(mod) == -1) {
			printf("nut failed to open: %s\n", dp->d_name);
			continue;
		}
	}
	closedir(dir);
	return 0;
}

void exit_nuts()
{
	struct nut *tmp;
	struct nut *del;
	tmp = nuts;
	while (tmp) {
		del = tmp;
		tmp = tmp->next;
		del->exit();
		if (del->name)
			free(del->name);
		if (del->module)
			unload_module(del->module);
		free(del);
	}
}

void *load_module(char *path)
{
    void *mod = dlopen(path, RTLD_NOW);
	dlerror();
	return mod;
}

void *get_symbol(void *ptr, char *sym)
{
	void *ret;
	// Reset errors
	dlerror();
	ret = dlsym(ptr, sym);
	const char* error = dlerror();
	if(error)
		return NULL;
	return ret;
}

void unload_module(void *ptr)
{
	dlclose(ptr);
}
