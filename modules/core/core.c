#include <nut.h>
#include <shell.h>

#include <stdio.h>

NUT_NAME("core")

int core_help(int argc, char *argv[])
{
	struct shell_cmd *c;
	c = cmd_list;
	printf("Available commands:\n");
	while (c) {
		printf("    %-20s%s\n", c->name, c->desc);
		c = c->next;
	}
	return 0;
}

int core_quit(int argc, char *argv[])
{
	printf("Quit!\n");
	return 1;
}

int core_init()
{
	register_command("quit", 'q', "Quit", &core_quit);
	register_command("help", 'h', "Help", &core_help);
	return 0;
}

void core_exit()
{
}

nut_init(core_init);
nut_exit(core_exit);
