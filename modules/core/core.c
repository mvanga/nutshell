#include <nut.h>
#include <shell.h>

#include <stdio.h>

NUT_NAME("core")

int core_quit(int argc, char *argv[])
{
	printf("Quit!\n");
	return 1;
}

int core_init()
{
	register_command("quit", 'q', "Quit", &core_quit);
	return 0;
}

void core_exit()
{
}

nut_init(core_init);
nut_exit(core_exit);
