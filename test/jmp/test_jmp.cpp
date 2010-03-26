#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <ctype.h>

namespace Test1
{

void first(void);
void second(void);

/* This program's output is:

calling first
calling second
entering second
second failed with type 3 exception; remapping to type 1.
first failed, exception type 1

*/

/* Use a file scoped static variable for the exception stack so we can access
 * it anywhere within this translation unit. */
static jmp_buf exception_env;
static int exception_type;

void go()
{
    volatile void *mem_buffer;

    mem_buffer = NULL;
    if (setjmp(exception_env))
    {
        /* if we get here there was an exception */
        printf("first failed, exception type %d\n", exception_type);
    }
    else
    {
        /* Run code that may signal failure via longjmp. */
        printf("calling first\n");
        first();
        mem_buffer = malloc(300); /* allocate a resource */
        printf(strcpy((char*) mem_buffer, "first succeeded!")); /* ... this will not happen */
    }
    if (mem_buffer)
    {
        free((void*) mem_buffer); /* carefully deallocate resource */
    }
}

void first(void)
{
    jmp_buf my_env;

    printf("calling second\n");
    memcpy(my_env, exception_env, sizeof(jmp_buf));
    switch (setjmp(exception_env))
    {
        case 3:
            /* if we get here there was an exception. */
            printf("second failed with type 3 exception; remapping to type 1.\n");
            exception_type = 1;

        default: /* fall through */
            memcpy(exception_env, my_env, sizeof(jmp_buf)); /* restore exception stack */
            longjmp(exception_env, exception_type); /* continue handling the exception */

        case 0:
            /* normal, desired operation */
            second();
            printf("second succeeded\n");  /* not reached */
    }
}

void second(void)
{
    printf("entering second\n" ); /* reached */
    exception_type = 3;
    longjmp(exception_env, exception_type); /* declare that the program has failed */
    printf("leaving second\n"); /* not reached */
}

}; // namespace Test1

namespace Test2
{

jmp_buf mainTask, childTask;

void call_with_cushion(void);
void child(void);

void go(void)
{
    if (!setjmp(mainTask))
    {
        call_with_cushion(); /* child never returns */ /* yield */
    } /* execution resumes after this "}" when child yields */
    for (;;)
    {
        printf("Parent\n");
        if (!setjmp(mainTask))
        {
            longjmp(childTask, 1); /* yield - note that this is undefined under C99 */
        }
    }
}


void call_with_cushion (void)
{
    char space[1000]; /* Reserve enough space for main to run */
    space[999] = 1; /* Do not optimize array out of existence */
    child();
}

void child (void)
{
    for (;;)
    {
        printf("Child loop begin\n");
        if (!setjmp(childTask)) longjmp(mainTask, 1); /* yield - invalidates childTask in C99 */

        printf("Child loop end\n");
        if (!setjmp(childTask)) longjmp(mainTask, 1); /* yield - invalidates childTask in C99 */
    }
    /* Don't return. Instead we should set a flag to indicate that main()
       should stop yielding to us and then longjmp(mainTask, 1) */
}

};

namespace Test3
{

//
int addsubt();
int multdiv();
int number();
void error();

//
static char expr[81];
static int pos;
static jmp_buf errjb;

void go()
{
    int ans;
    do
    {
        if (setjmp(errjb) == 0)
        {
            //
            pos = 0;

            //
            printf("Enter expression (0 to quit):\n");
            scanf("%s", expr);

            ans = addsubt();
            if (expr[pos] != '\0')
                error();
            if (ans != 0)
                printf("%d\n", ans);
        }
        else
        {
            //
            printf("Try again\n");
            ans = 1;
        }
    }
    while (ans != 0);
}

//
int addsubt()
{
    int rtn = multdiv();
    while (expr[pos]=='+' || expr[pos]=='-')
    {
        int op = expr[pos++];
        int opr2 = multdiv();
        if (op == '+')
            rtn += opr2;
        else
            rtn -= opr2;
    }

    return rtn;
}

//
int multdiv()
{
    int rtn = number();
    while (expr[pos]=='*' || expr[pos]=='/')
    {
        int op = expr[pos++];
        int opr2 = number();
        if (op == '*')
            rtn *= opr2;
        else
            rtn /= opr2;
    }

    return rtn;
}

//
int number()
{
    int rtn;

    if (expr[pos] == '(')
    {
        pos++;
        rtn = addsubt();
        if (expr[pos++]!=')')
            error();
        return rtn;
    }

    //
    if (!isdigit(expr[pos]))
        error();
    rtn = atoi(expr+pos);
    while (isdigit(expr[pos]))
        pos++;

    return rtn;
}

void error()
{
    printf("\r");

    while (pos--)
        printf(" ");
    printf("^ syntax error\n\a");

    //
    longjmp(errjb, 1);
}

}; // namespace Test3

int main()
{
    Test1::go();

    Test2::go();


    Test3::go();

    return 0;
}
