// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "read_file.h"

#include <new>

void __cdecl break_new_handler()
{
    _asm { int 3 }
}

#include "tests/pairutil_test.h"
#include "tests/sizes_test.h"
#include "tests/reader_test.h"
#include "tests/test_utils.h"

#include <stdio.h>
#include <string>

enum Option { TEST = 0, LISTING = 1, COMPILE = 2 };

void usage(const char *program)
{
    printf("Usage: %s\n"
           "         to run tests\n"
           " -or-  %s -compile [-listing] <source-file>\n", 
           "         to compile a source file\n", program, program);
    exit(-1);
}

unsigned options(unsigned argc, const char **argv, std::string &filename)
{
    unsigned option = TEST;
    for (unsigned i = 1; i < argc; i++)
    {
        const char *opt = argv[i];
        if      (opt && stricmp(opt, "-listing") == 0) option |= LISTING;
        else if (opt && stricmp(opt, "-compile") == 0) option |= COMPILE;
        else if (opt && *opt == '-') usage(argv[0]);
        else if (opt && *opt) filename = opt;
    }
    if (option != TEST && filename.empty())
        usage(argv[0]);
    return option;
}

#include "read_file.h"
#include "object.h"
#include "reader.h"
#include "streams.h"
#include "format.h"

#include "compiler/code.h"
#include "compiler/scriptcompiler.h"
#include "disassembler/disassembler.h"

int compile(const char *program, const char *filename, unsigned options)
{
    file::buffer_t file = file::read_file(filename);
    if (!file.data || !file.size)
    {
        fprintf(stderr, "Can't find file or file is empty.\n");
        usage(program);
    }

    Reader reader;
    Object *e = reader.read((const char *)file.data);
    ScriptCompiler sc(Object::cast_pair(e), true);

    Code *c = sc.compile_code();

    int r = 0;

    if (!c)
    {
        printf("\nCompile failed!\n");
        r = -1;
    }
    else
    {
        FileOutStream out(format::format("%s.sbin", filename).c_str());
        c->write(out);
        out.close();

        if ((options & LISTING) != 0)
        {
            std::string s = Disassembler::disassemble(c);
            FileOutStream listing(format::format("%s.lst", filename).c_str());
            listing.write(s.data(), s.size());
            listing.close();
        }
    }

    delete [] file.data;

    return r;
}

int main(int argc, const char **argv)
{
    std::set_new_handler(break_new_handler);

    std::string filename;
    unsigned ops = options(argc, argv, filename);
    int r = 0;
    if (ops != TEST)
        r = compile(argv[0], filename.c_str(), ops);
    else
    {
        pairutil_test();
        sizes_test();
        reader_test();
        wait_console();
    }
    return r;
}