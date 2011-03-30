#ifndef __VMEIO_TEST_SHELL_H
#define __VMEIO_TEST_SHELL_H

typedef int (*command_t)(int, char **);

struct shell_cmd {
	char *name;
	char sname;
	char *desc;
	command_t cmd;
	struct shell_cmd *next;
};

extern struct shell_cmd *cmd_list;

int register_command(char *name, char sname, char *desc, command_t cmd);
struct shell_cmd *get_command(char *name);
struct shell_cmd *get_command_char(char name);
void free_commands();

#endif
