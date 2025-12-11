#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"

/* extern example: defined here, visible elsewhere via extern */
char *shell_prompt = "myshell> ";

int builtin_cd(int argc, char **argv)
{
    const char *path = (argc > 1 ? argv[1] : getenv("HOME"));
    if (chdir(path) != 0) {
        perror("cd");
        return 1;
    }
    return 0;
}

int builtin_exit(int argc, char **argv)
{
    (void)argc; (void)argv;
    printf("Exiting shell...\n");
    exit(0);
}

int builtin_help(int argc, char **argv)
{
    (void)argc; (void)argv;
    printf("Built-ins: cd, exit, help, prompt, load, py\n");
    printf("Use 'load text' or 'load math' to load plugins.\n");
    return 0;
}

int builtin_prompt(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: prompt <new_prompt>\n");
        return 1;
    }
    shell_prompt = argv[1];
    printf("Prompt set to: %s\n", shell_prompt);
    return 0;
}
