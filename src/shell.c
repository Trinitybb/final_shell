#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "cmdiface.h"

#define MAX_ARGS 32

CmdPlugin *loaded_plugins[10];
int plugin_count = 0;

int load_plugin(char *name)
{
    char errmsg[256];
    CmdPlugin *p = cmdiface_bind(name, errmsg);
    if (!p) {
        fprintf(stderr, "Plugin load failed: %s\n", errmsg);
        return 1;
    }
    loaded_plugins[plugin_count++] = p;
    printf("Loaded plugin: %s\n", name);
    return 0;
}

int run_python(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: py <args>\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        char *args[40];
        args[0] = "python3";
        args[1] = "scripts/analyze.py";
        for (int i = 1; i < argc; i++) args[i+1] = argv[i];
        args[argc+1] = NULL;
        execvp("python3", args);
        perror("execvp");
        exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    return 0;
}

int run_command(int argc, char **argv)
{
    if (argc == 0) return 0;

    // builtins:
    if (strcmp(argv[0], "cd") == 0) return builtin_cd(argc, argv);
    if (strcmp(argv[0], "exit") == 0) return builtin_exit(argc, argv);
    if (strcmp(argv[0], "help") == 0) return builtin_help(argc, argv);
    if (strcmp(argv[0], "load") == 0) return load_plugin(argv[1]);
    if (strcmp(argv[0], "py") == 0) return run_python(argc, argv);

    // plugin commands:
    for (int p=0; p < plugin_count; p++) {
        for (size_t i=0; i < loaded_plugins[p]->num_cmds; i++) {
            CmdIface *cmd = &loaded_plugins[p]->cmds[i];
            if (strcmp(cmd->name(), argv[0]) == 0)
                return cmd->run(argc, argv);
        }
    }

    printf("Unknown command: %s\n", argv[0]);
    return 1;
}

int main()
{
    char line[256];

    while (1) {
        printf("%s", shell_prompt);
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) break;

        char *args[MAX_ARGS];
        int argc = 0;

        char *t = strtok(line, " \t\n");
        while (t && argc < MAX_ARGS) {
            args[argc++] = t;
            t = strtok(NULL, " \t\n");
        }

        run_command(argc, args);
    }

    return 0;
}
