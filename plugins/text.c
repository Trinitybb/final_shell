#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmdiface.h"

/* Metadata */
char *plugin_name    = "Text Utilities Plugin";
char *plugin_version = "1.0";
char *iface_version  = "1.0";

/* Command 1: words */
static int words_run(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: words <string>\n");
        return 1;
    }
    int count = 0, in_word = 0;
    for (char *p = argv[1]; *p; p++) {
        if (!isspace((unsigned char)*p) && !in_word) {
            in_word = 1;
            count++;
        } else if (isspace((unsigned char)*p)) {
            in_word = 0;
        }
    }
    printf("Words: %d\n", count);
    return 0;
}
static const char *words_name(void) { return "words"; }
static const char *words_help(void) { return "Count words in a string."; }

/* Command 2: upper */
static int upper_run(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: upper <string>\n");
        return 1;
    }
    for (char *p = argv[1]; *p; p++) {
        putchar(toupper((unsigned char)*p));
    }
    putchar('\n');
    return 0;
}
static const char *upper_name(void) { return "upper"; }
static const char *upper_help(void) { return "Convert string to uppercase."; }

/* Command 3: reverse */
static int reverse_run(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: reverse <string>\n");
        return 1;
    }
    char *s = argv[1];
    for (int i = (int)strlen(s) - 1; i >= 0; i--) {
        putchar(s[i]);
    }
    putchar('\n');
    return 0;
}
static const char *reverse_name(void) { return "reverse"; }
static const char *reverse_help(void) { return "Reverse a string."; }

/* Command 4: vowels */
static int vowels_run(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: vowels <string>\n");
        return 1;
    }
    int count = 0;
    for (char *p = argv[1]; *p; p++) {
        char c = (char)tolower((unsigned char)*p);
        if (c=='a'||c=='e'||c=='i'||c=='o'||c=='u') count++;
    }
    printf("Vowels: %d\n", count);
    return 0;
}
static const char *vowels_name(void) { return "vowels"; }
static const char *vowels_help(void) { return "Count vowels in a string."; }

/* Command 5: piglatin */
static int piglatin_run(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: piglatin <word>\n");
        return 1;
    }
    char *s = argv[1];
    if (strchr("aeiouAEIOU", s[0])) {
        printf("%say\n", s);
    } else {
        printf("%s%cay\n", s+1, s[0]);
    }
    return 0;
}
static const char *piglatin_name(void) { return "piglatin"; }
static const char *piglatin_help(void) { return "Convert word to Pig Latin."; }

/* Exported table */
CmdIface text_cmds[] = {
    { words_run,    words_name,    words_help },
    { upper_run,    upper_name,    upper_help },
    { reverse_run,  reverse_name,  reverse_help },
    { vowels_run,   vowels_name,   vowels_help },
    { piglatin_run, piglatin_name, piglatin_help }
};

size_t text_num_cmds = sizeof(text_cmds) / sizeof(text_cmds[0]);
