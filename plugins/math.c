#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cmdiface.h"

/* Metadata */
char *plugin_name    = "Math Utilities Plugin";
char *plugin_version = "1.0";
char *iface_version  = "1.0";

/* factorial */
static long long factorial(int n)
{
    if (n < 0) return -1;
    long long r = 1;
    for (int i = 2; i <= n; i++) r *= i;
    return r;
}

/* fibonacci */
static long long fibonacci(int n)
{
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    long long a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

/* helper: gcd */
static int gcd_int(int a, int b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* helper: prime */
static int is_prime(int n)
{
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

/* Command: fact */
static int fact_run(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: fact <n>\n");
        return 1;
    }
    int n = atoi(argv[1]);
    long long r = factorial(n);
    printf("factorial(%d) = %lld\n", n, r);
    return 0;
}
static const char *fact_name(void) { return "fact"; }
static const char *fact_help(void) { return "Compute factorial of n."; }

/* Command: fib */
static int fib_run(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: fib <n>\n");
        return 1;
    }
    int n = atoi(argv[1]);
    long long r = fibonacci(n);
    printf("fibonacci(%d) = %lld\n", n, r);
    return 0;
}
static const char *fib_name(void) { return "fib"; }
static const char *fib_help(void) { return "Compute Fibonacci(n)."; }

/* Command: gcd */
static int gcd_run(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: gcd <a> <b>\n");
        return 1;
    }
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    printf("gcd(%d, %d) = %d\n", a, b, gcd_int(a, b));
    return 0;
}
static const char *gcd_name(void) { return "gcd"; }
static const char *gcd_help(void) { return "Compute greatest common divisor."; }

/* Command: isprime */
static int prime_run(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: isprime <n>\n");
        return 1;
    }
    int n = atoi(argv[1]);
    printf("%d is %sprime\n", n, is_prime(n) ? "" : "not ");
    return 0;
}
static const char *prime_name(void) { return "isprime"; }
static const char *prime_help(void) { return "Test if n is prime."; }

/* Exported table */
CmdIface math_cmds[] = {
    { fact_run,  fact_name,  fact_help },
    { fib_run,   fib_name,   fib_help },
    { gcd_run,   gcd_name,   gcd_help },
    { prime_run, prime_name, prime_help }
};

size_t math_num_cmds = sizeof(math_cmds) / sizeof(math_cmds[0]);
