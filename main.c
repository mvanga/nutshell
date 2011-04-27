#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef WITH_READLINE
#include <readline.h>
#endif

#include <shell.h>
#include <nut.h>

#define NUTSHELL_VERSION "1.0"
#define BUFSIZE 1024

#define ERROR "error: "
#define WARN "warning: "

char *readline (const char *);
void add_history (const char *);

static int setargs(char *args, char **argv)
{
	int count = 0;

	while (isspace(*args))
		++args;
	while (*args) {
		if (argv)
			argv[count] = args;
		while (*args && !isspace(*args))
			++args;
		if (argv && *args)
			*args++ = '\0';
		while (isspace(*args))
			++args;
		count++;
	}
	return count;
}

char **parse_args(char *args, int *argc)
{
	char **argv = NULL;
	int    argn = 0;
	if (args && *args
		&& (args = strdup(args))
		&& (argn = setargs(args,NULL))
		&& (argv = malloc((argn+1) * sizeof(char *)))) {
		*argv++ = args;
		argn = setargs(args,argv);
	}
	if (args && !argv)
		free(args);
	*argc = argn;
	return argv;
}

void free_args(char **argv)
{
	if (argv) {
		free(argv[-1]);
		free(argv-1);
	} 
}

int cexec(char *buffer)
{
	int ac;
	char **av;
	int run = 0;
	struct shell_cmd *cmd;
	av = parse_args(buffer, &ac);
	if (ac < 1) {
		free_args(av);
		return 0;;
	}
	/* look for the command by name and char name */
	cmd = get_command(av[0]);
	if (!cmd && strlen(av[0]) == 1)
		cmd = get_command_char(av[0][0]);
	if (!cmd) {
		printf("%s: command not found\n", av[0]);
		free_args(av);
		return 0;
	}
	/* if a command returns non-zero, it means we should exit */
	run = cmd->cmd(ac, av);
	free_args(av);

	return run;
}

int run_script(char *filename, int script)
{
	FILE *f;
	int run;
	char *str;
	char buffer[BUFSIZE];
	f = fopen(filename, "r");
	if (!f) {
		if (script)
			printf("failed to open nutshell script: %s\n", filename);
		return 1;
	}
	while (run && fgets(buffer, sizeof(buffer), f) != NULL) {
		/* get rid of trailing newline if any */
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';
		/* run the commands in the line */
		str = strtok(buffer, ";");
		run = !cexec(str);
		while ((str = strtok(NULL, ";")) != NULL)
			run = !cexec(str);
	}
	fclose(f);
	return 0;
}

int main(int argc, char *argv[])
{
	int run = 1;
	char *str;
#if WITH_LIBREADLINE
	char *buffer = NULL;
#else
	char buffer[BUFSIZE];
#endif

	/* we have a script being called */
	int i;
	if (argc > 1) {
		for (i = 1; i < argc; i++)
			run_script(argv[i], 1);
		return 0;
	}

	init_nuts();

	/* interactive mode */
	printf("Nutshell (Version %s)\n", NUTSHELL_VERSION);
	run_script(".nutrc", 0);
	/* go into shell loop */
	do {
#if WITH_LIBREADLINE
		if (buffer) {
			free (buffer);
			buffer = (char *)NULL;
		}
		buffer = readline ("> ");
		if (buffer && *buffer)
			add_history (buffer);
#else
		printf("> ");
		fgets(buffer, BUFSIZE - 1, stdin);
		/* get rid of trailing newline if any */
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';
#endif
		if (strlen(buffer) == 0)
			continue;
		/* run the commands */
		str = strtok(buffer, ";");
		run = !cexec(str);
		while ((str = strtok(NULL, ";")) != NULL)
			run = !cexec(str);
		strcpy(buffer, "");
	} while (run);
	free_commands();
	exit_nuts();
	return 0;
}
