// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "nativecaller.h"
#include "types.h"

static const unsigned char _template_prologue[] = 
{
    // 55               push        ebp  
    // 8B EC            mov         ebp, esp

    0x55,
    0x8b, 0xec
};

static const unsigned char _template_unbox[] = 
{
    // 8B 4D 10         mov         ecx, [array]
    // FF 71 xx         push        [ecx + (array-index * 4)]
    // 8B 55 08         mov         edx, [converter-struct]
    // FF 52 xx         call        [edx + (converter-fn-index * 4)]
    // (alternatively for converter-fn-index == 0)
    // FF 12            call        [edx]


    0x8b, 0x4d, 0x10,
    0xff, 0x71,    0,
    0x8b, 0x55, 0x08,
    0xff, 0x52,    0
};

static const unsigned char _template_push_float[] =
{
    // D9 1C 24         fstp        [esp]

    0xd9, 0x1c, 0x24
};

static const unsigned char _template_push_int4[] =
{
    // 83 C4 04         add         esp,4 
    // 50               push        eax  

    0x83, 0xc4, 0x04,
    0x50
};

static const unsigned char _template_call[] =
{
    // FF 55 0C         call        [fn]

    0xff, 0x55, 0x0c
};

static const unsigned char _template_clean_stack[] =
{
    // 83 C4 xx         add         esp, (n * 4) // only if n > 0

    0x83, 0xc4, 0
};

static const unsigned char _template_epilogue[] =
{
    // 5D               pop         ebp  
    // C3               ret            

    0x5d,
    0xc3
};

void NativeCaller::m_unbox(unsigned char **c, unsigned i, Converter conversion)
{
    memcpy(*c, _template_unbox, sizeof(_template_unbox));
    (*c)[5]  = (unsigned char)(i * 4);
    (*c)[11] = (unsigned char)(conversion * 4);
    (*c) += sizeof(_template_unbox);
    const unsigned char *push_code = 0;
    unsigned             push_sz   = 0;
    switch (conversion)
    {
    case CONVERT_FLOAT: push_code = _template_push_float; push_sz = sizeof(_template_push_float); break;
    default:            push_code = _template_push_int4;  push_sz = sizeof(_template_push_int4);  break;
    }
    memcpy(*c, push_code, push_sz);
    (*c) += push_sz;
}

void NativeCaller::m_unbox(char format_char, unsigned char **code, unsigned count)
{
    switch (format_char)
    {
    case 'f': m_unbox(code, count, CONVERT_FLOAT);   break;
    case 'u': m_unbox(code, count, CONVERT_UINT);    break;
    case 's': m_unbox(code, count, CONVERT_STRING);  break;
    case 'p': m_unbox(code, count, CONVERT_POINTER); break;
    }
}

void NativeCaller::m_create_function(unsigned char *code, const char *format, CallConvention convention)
{
    memcpy(code, _template_prologue, sizeof(_template_prologue));
    code += sizeof(_template_prologue);
    // write argument unboxing code
	unsigned sz = (unsigned)strlen(format), count = sz; // number of args
    while (*format)
    {
        --count;
        m_unbox(*format, &code, count);
        format++;
    } 
    // write call
    memcpy(code, _template_call, sizeof(_template_call));
    code += sizeof(_template_call);
    // write stack clean-up if necessary
    if (sz && convention != CALL_STDCALL)
    {
        memcpy(code, _template_clean_stack, sizeof(_template_clean_stack));
        code[2] = (unsigned char)(sz * 4);
        code += sizeof(_template_clean_stack);
    }
    // write epilogue - function return
    memcpy(code, _template_epilogue, sizeof(_template_epilogue));
    code += sizeof(_template_epilogue);
}

Object *NativeCaller::m_call_void(const unsigned char *code, const Converters &converters, void *function, Object **array)
{
	caller_void_t fn = (caller_void_t)(code);
	fn(converters, function, array);
	return 0;//Types::create_bool(false);
}

Object *NativeCaller::m_call_float(const unsigned char *code, const Converters &converters, void *function, Object **array)
{
	caller_float_t fn = (caller_float_t)(code);
    return (Object *)Types::create_float(fn(converters, function, array));
}

Object *NativeCaller::m_call_uint(const unsigned char *code, const Converters &converters, void *function, Object **array)
{
	caller_uint_t fn = (caller_uint_t)(code);
    return (Object *)Types::create_int((int)fn(converters, function, array));
}

Object *NativeCaller::m_call_string(const unsigned char *code, const Converters &converters, void *function, Object **array)
{
	caller_string_t fn = (caller_string_t)(code);
    return (Object *)Types::create_string(fn(converters, function, array));
}

Object *NativeCaller::m_call_symbol(const unsigned char *code, const Converters &converters, void *function, Object **array)
{
    caller_string_t fn = (caller_string_t)(code);
    return (Object *)Types::create_symbol(fn(converters, function, array));
}

Object *NativeCaller::m_call_pointer(const unsigned char *code, const Converters &converters, void *function, Object **array)
{
	caller_pointer_t fn = (caller_pointer_t)(code);
    return Types::create_pointer(fn(converters, function, array));
}

NativeCaller::NativeCaller(void *function, CallConvention convention, char out, const char *format) : 
    m_function(function), m_typed_caller(0), m_argcount(0)
{
    m_argcount = strlen(format);
	m_create_function(m_code, format, convention);
    m_converters.to_float   = Types::to_float;
	m_converters.to_uint    = Types::to_uint;
    m_converters.to_string  = Types::to_cstr;
    m_converters.to_pointer = Types::to_pointer;
	switch (out)
	{
    case 'v': m_typed_caller = m_call_void;    break;
    case 'f': m_typed_caller = m_call_float;   break;
    case 'u': m_typed_caller = m_call_uint;    break;
    case 's': m_typed_caller = m_call_string;  break;
    case 'x': m_typed_caller = m_call_symbol;  break;
    case 'p': m_typed_caller = m_call_pointer; break;
	}
}