#include "parse.h"#include "global.h"#include <stdio.h>#include <stdlib.h>#include <string.h>bool GetBool(const char *s, bool defaultValue){    if (s)    {        if      (strcmp(s, "true")  == 0)  return true;        else if (strcmp(s, "false") == 0) return false;    }    return defaultValue;}bool GetRequiredBool(const char *s, const char *name){    if (s)    {        if      (strcmp(s, "true")  == 0)  return true;        else if (strcmp(s, "false") == 0) return false;    }    if (name)            globalErrorHandler("Expected 'true' or 'false' for boolean attribute '%s'", name);    else        globalErrorHandler("Expected 'true' or 'false' for boolean value!");    return false;}float GetRequiredFloat(const char *s, const char *name){    if (s)    {        char *e = (char *)s;        float v = (float)strtod(s, &e);        if (e && s != e)            return v;    }    if (name)        globalErrorHandler("Expected numeric value for float attribute '%s'", name);    else        globalErrorHandler("Expected numeric value for float!");        return 0.0f;}float GetFloat(const char *s, float defaultValue){    if (s)    {        char *e = (char *)s;        float v = (float)strtod(s, &e);        if (e && s != e)            return v;    }    return defaultValue;}int GetRequiredInt(const char *s, const char *name){    if (s)    {        char *e = (char *)s;        int v = (int)strtol(s, &e, 10);        if (e && s != e)            return v;    }    if (name)        globalErrorHandler("Expected integer value for integer attribute '%s'", name);    else        globalErrorHandler("Expected integer value!");    return 0;}int GetInt(const char *s, int defaultValue){    if (s)    {        char *e = (char *)s;        int v = (int)strtol(s, &e, 10);        if (e && s != e)            return v;    }    return defaultValue;}