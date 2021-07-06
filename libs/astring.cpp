#include "../include/astring.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

//-------------------------------------------------------------------------------------------------
// Деструктор

AString::~AString ()
{
    if (p)
        free (p);
}

//-------------------------------------------------------------------------------------------------
// Напечатать в динамически выделяемый буфер

bool AString::printf (const char* fmt, ...)
{
    if (p)
        free (p);
    p = nullptr;
    va_list arglist;
    va_start (arglist, fmt);
    int res = vasprintf (&p, fmt, arglist);
    va_end (arglist);
    len = res >= 0 ? res : 0;
    return res >= 0;
}

//-------------------------------------------------------------------------------------------------
// Скопировать из С-строки

bool AString::assign (const char* str)
{
    return printf ("%s", str);
}

//-------------------------------------------------------------------------------------------------
//
