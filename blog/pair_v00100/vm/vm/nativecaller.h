// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _NATIVECALLER_H
#define _NATIVECALLER_H

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <conio.h>

#include "util.h"

class Object;

class NativeCaller
{
public:

    enum CallConvention { CALL_STDCALL, CALL_CDECL };

	NativeCaller(void *function, CallConvention convention, char out, const char *format);

    inline unsigned arguments() const
    {
        return m_argcount;
    }

	inline Object *call(Object **array)
	{
        Util::reverse(array, m_argcount); // $todo: do this the right way
		return m_typed_caller(&m_code[0], m_converters, m_function, array);
	}

    inline void *data() { return (void *)m_code; }

private:

	enum { MAX_FUNCTION_SIZE = 512 };

	unsigned char   m_code[MAX_FUNCTION_SIZE];
	void           *m_function;
    unsigned        m_argcount;

    struct Converters
    {
        float       (*to_float)  (Object *);
	    unsigned    (*to_uint)   (Object *);
	    const char *(*to_string) (Object *);
	    void       *(*to_pointer)(Object *);
    };
    // these must reflect the order of the above members
    enum Converter { CONVERT_FLOAT = 0, CONVERT_UINT = 1, CONVERT_STRING = 2, CONVERT_POINTER = 3 }; 

	typedef Object     *(*typed_caller)     (const unsigned char *, const Converters &, void *, Object **);

    typedef void        (*caller_void_t)    (const Converters &, void *, Object **);
    typedef float       (*caller_float_t)   (const Converters &, void *, Object **);
	typedef unsigned    (*caller_uint_t)    (const Converters &, void *, Object **);
	typedef const char *(*caller_string_t)  (const Converters &, void *, Object **);
	typedef void       *(*caller_pointer_t) (const Converters &, void *, Object **);

	Converters         m_converters;
	typed_caller       m_typed_caller;

	static void        m_unbox(unsigned char **c, unsigned i, Converter converter);
    static void        m_unbox(char format_char, unsigned char **code, unsigned count);
	static void        m_create_function(unsigned char *code, const char *format, CallConvention convention);

	static Object     *m_call_void   (const unsigned char *code, const Converters &converters, void *function, Object **array);
	static Object     *m_call_float  (const unsigned char *code, const Converters &converters, void *function, Object **array);
	static Object     *m_call_uint   (const unsigned char *code, const Converters &converters, void *function, Object **array);
	static Object     *m_call_string (const unsigned char *code, const Converters &converters, void *function, Object **array);
	static Object     *m_call_symbol (const unsigned char *code, const Converters &converters, void *function, Object **array);
	static Object     *m_call_pointer(const unsigned char *code, const Converters &converters, void *function, Object **array);
};

#endif  // _NATIVECALLER_H