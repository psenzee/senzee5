#ifndef _CORE_ASSERT_H
#define _CORE_ASSERT_H

#ifdef WIN32
#include <conio.h>
#endif

#include <stdio.h>
#include <string.h>

inline void core_wait(const char *message)
{
#ifdef WIN32
    if (!message)
        message = "Press Any Key..";
    printf("%s\n", message);
    getch();
#endif
}

inline void core_error(const char *text, const char *file, int line)
{
    printf("Assert '%s' in file '%s' at line '%d'!\n", text, file, line);
#ifdef WIN32
    __asm int 3;
    getch();
#endif
}

#define core_assert(cond)                { if (!(cond)) core_error(#cond, __FILE__, __LINE__); } // define this for release only..
#define core_abort(formatted_expression) { printf formatted_expression; core_error("[ABORT]", __FILE__, __LINE__); exit(-1); }

#endif  // _CORE_ASSERT_H