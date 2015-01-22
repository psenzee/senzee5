// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>

#include "../object.h"
#include "../vm.h"
#include "../scriptvm.h"
#include "../string.h"
#include "../numbert.h"
#include "../pointer.h"
#include "../exceptions.h"
#include "../streams.h"
#include "../format.h"
#include "../ffifunction.h"
#include "../types.h"

namespace function
{

static __forceinline Object *_arg(Object **objects, unsigned index, unsigned count)
{
    return objects[(count - 1) - index];
}

Object *concat(Object **objects, unsigned count)
{
    std::string ss; // needs to be more efficient
    for (unsigned i = 0; i < count; i++)
        ss += Object::str(_arg(objects, i, count));
    return Types::create_string(ss);
}

Object *string(Object **objects, unsigned count)
{
    return Types::create_string(Object::str(objects[0]));
}

StaticPointer _standard_out(type_standard_out_stream, 0),
              _standard_err(type_standard_out_stream, 0);

Object *standard_out(Object **objects, unsigned count)
{
    static StdOutStream s(stdout);
    if (!_standard_out.pointer)
        _standard_out.pointer = &s;
    return &_standard_out;
}

Object *standard_error(Object **objects, unsigned count)
{
    static StdOutStream s(stderr);
    if (!_standard_err.pointer)
        _standard_err.pointer = &s;
    return &_standard_err;
}

Object *open_out_file(Object **objects, unsigned count)
{
    std::string flags(count > 1 ? Object::str(_arg(objects, 1, count)) : "wb");
    FILE *file = fopen(Object::str(_arg(objects, 0, count)).c_str(), flags.c_str());
    return file ? Types::create_pointer(type_file_out_stream, new FileOutStream(file)) : 0;
}

Object *open_out_string(Object **objects, unsigned count)
{
    return _gc_new(new StringPointer<StringOutStream>(type_string_out_stream, new StringOutStream));
}

Object *open_in_file(Object **objects, unsigned count)
{
    std::string flags(count > 1 ? Object::str(_arg(objects, 1, count)) : "rb");
    FILE *file = fopen(Object::str(_arg(objects, 0, count)).c_str(), flags.c_str());
    return file ? Types::create_pointer(type_file_in_stream, new FileInStream(file)) : 0;
}

Object *write(Object **objects, unsigned count)
{
    IOutStream *stream = (IOutStream *)cast_to_raw_pointer(_arg(objects, 0, count), type_out_stream);
    if (!stream) return 0;
    for (unsigned i = 1; i < count; i++)
        if (!write_format(*stream, "%s", Object::str(_arg(objects, i, count)).c_str()))
            return 0;
    return Vm::VM_TRUE;
}

Object *flush(Object **objects, unsigned count)
{
    IOutStream *stream = (IOutStream *)cast_to_raw_pointer(objects[0], type_out_stream);
    if (!stream) return 0;
    stream->flush();
    return Vm::VM_TRUE;
}

Object *close(Object **objects, unsigned count)
{
    IStream *stream = (IStream *)cast_to_raw_pointer(objects[0], type_stream);
    if (stream) stream->close();
    return stream ? Vm::VM_TRUE : 0;
}

static bool _verify_return(const std::string &s)
{
    if (s.size() != 1)
        return false;
    char c = s[0];
    return c == 'v' || c == 'u' || c == 'f' || c == 's' || c == 'p';
}

static bool _verify_format(const std::string &s)
{
    if (s.empty())
        return true;
    for (std::string::const_iterator i = s.begin(), e = s.end(); i != e; ++i)
    {
        char c = *i;
        if (c == 'u' || c == 'f' || c == 's' || c == 'p')
            return true;
    }
    return false;
}

Object *import(Object **objects, unsigned count)
{
    enum { LIBRARY_NAME = 0, CALL_CONV = 1, FUNCTION_NAME = 2, RETURN_TYPE = 3, ARG_TYPES = 4, EXPECTED_ARGS };
    if (count < EXPECTED_ARGS)
    {
        printf("\n** import failure: expected %u args, found %u\n", EXPECTED_ARGS, count);
        return 0;
    }
    Object *u = 0;
    std::string s[EXPECTED_ARGS];
    for (unsigned i = 0; i < EXPECTED_ARGS; i++)
    {
        if (!(u = _arg(objects, i, count)))
        {
            printf("\n** import failure: arg %u is nil.\n", i);
            return 0;
        }
        s[i] = Object::str(u);
    }

    // args preprocessed, now load..
    NativeCaller::CallConvention cc = NativeCaller::CALL_CDECL;
    const char *ccstr = s[CALL_CONV].c_str();
    if (strcmp(ccstr, "stdcall") == 0)
        cc = NativeCaller::CALL_STDCALL;
    else if (strcmp(ccstr, "cdecl") != 0)
    {
        printf("\n** import failure: expected symbol 'stdcall' or 'cdecl' calling convention [arg %u].\n", CALL_CONV + 1);
        return 0;
    }
    if (!_verify_return(s[RETURN_TYPE]))
    {
        printf("\n** import failure: return value type [arg %u] should be a single character, of [v, u, f, s, p].\n", RETURN_TYPE + 1);
        return 0;
    }
    if (!_verify_format(s[ARG_TYPES]))
    {
        printf("\n** import failure: arg list format [arg %u] must be a string of characters, each of [u, f, s, p].\n", ARG_TYPES + 1);
        return 0;
    }
    HMODULE handle = LoadLibrary(s[LIBRARY_NAME].c_str());
    if (!handle)
    {
        printf("\n** import failure: unable to open dll %s [arg %u].\n", s[LIBRARY_NAME].c_str(), LIBRARY_NAME + 1);
        return 0;
    }
    FARPROC proc = GetProcAddress(handle, s[FUNCTION_NAME].c_str());
    if (!proc)
    {
        printf("\n** import failure: unable to find procedure %s [arg %u].\n", s[FUNCTION_NAME].c_str(), FUNCTION_NAME + 1);
        FreeLibrary(handle);
        return 0;
    }
    return new NativeFunction(s[FUNCTION_NAME].c_str(), proc, cc, s[RETURN_TYPE][0], s[ARG_TYPES].c_str());
}

void register_common_functions(ScriptVm &svm)
{
    svm.registerfunction("concat",                   function::concat,                   2, -1);
    svm.registerfunction("string",                   function::string,                   1,  1);

    svm.registerfunction("open-out-file",            function::open_out_file,            1,  2);
    svm.registerfunction("open-in-file",             function::open_in_file,             1,  2);

    svm.registerfunction("open-out-string",          function::open_out_string,          0,  0);

    svm.registerfunction("standard-out",             function::standard_out,             0,  0);
    svm.registerfunction("standard-error",           function::standard_error,           0,  0);

    svm.registerfunction("write",                    function::write,                    2, -1);
    svm.registerfunction("flush",                    function::flush,                    1,  1);
    svm.registerfunction("close",                    function::close,                    1,  1);

    svm.registerfunction("import",                   function::import,                   5,  5);
}

}