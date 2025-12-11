#ifndef BUILTINS_H
#define BUILTINS_H

/* extern example: shared global prompt string */
extern char *shell_prompt;

int builtin_cd(int argc, char **argv);
int builtin_exit(int argc, char **argv);
int builtin_help(int argc, char **argv);
int builtin_prompt(int argc, char **argv);

#endif
