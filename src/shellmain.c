#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "builtins.h"
#include "cmdiface.h"
#include "util.h"

#define MAX_ARGS  32
#define MAX_PLUGS 8

static CmdPlugin *loaded_plugins[MAX_PLUGS];
static int plugin_count = 0;

/* Load a plugin: 'load text' looks for libtext.so */
static int cmd_load(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: load <module>\n");
        return 1;
    }
    if (plugin_count >= MAX_PLUGS) {
        printf("Maximum plugins loaded.\n");
        return 1;
    }

    char errmsg[256];
    CmdPlugin *p = cmdiface_bind(argv[1], errmsg);
    if (!p) {
        fprintf(stderr, "Error loading plugin '%s': %s\n", argv[1], errmsg);
        return 1;
    }

    loaded_plugins[plugin_count++] = p;
    printf("Loaded plugin '%s' v%s (iface %s)\n",
           p->plugin_name, p->plugin_version, p->iface_version);
    return 0;
}

/* Run python through a script */
static int cmd_py(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: py <args...>\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        char *args[64];
        int i = 0;
        args[i++] = "python3";
        args[i++] = "scripts/analyze.py";
        for (int j = 1; j < argc && i < 63; j++) {
            args[i++] = argv[j];
        }
        args[i] = NULL;
        execvp("python3", args);
        perror("execvp python3");
        _exit(1);
    } else if (pid < 0) {
        perror("fork");
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}

/* dispatch commands: builtins, py, load, then plugins, then error */
static int run_command(int argc, char **argv)
{
    if (argc == 0) return 0;

    /* Built-ins first */
    if (strcmp(argv[0], "cd") == 0)     return builtin_cd(argc, argv);
    if (strcmp(argv[0], "exit") == 0)   return builtin_exit(argc, argv);
    if (strcmp(argv[0], "help") == 0)   return builtin_help(argc, argv);
    if (strcmp(argv[0], "prompt") == 0) return builtin_prompt(argc, argv);
    if (strcmp(argv[0], "load") == 0)   return cmd_load(argc, argv);
    if (strcmp(argv[0], "py") == 0)     return cmd_py(argc, argv);

    /* Plugin commands: search all loaded plugins */
    for (int p = 0; p < plugin_count; p++) {
        CmdPlugin *plug = loaded_plugins[p];
        for (size_t i = 0; i < plug->num_cmds; i++) {
            CmdIface *cmd = &plug->cmds[i];
            if (strcmp(cmd->name(), argv[0]) == 0) {
                return cmd->run(argc, argv);
            }
        }
    }

    printf("Unknown command: %s\n", argv[0]);
    return 1;
}

int main(void)
{
    char line[256];
    char *argv[MAX_ARGS];

    while (1) {
        printf("%s", shell_prompt);
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) break;

        int argc = split_line(line, argv, MAX_ARGS);
        run_command(argc, argv);
    }

    return 0;
}
