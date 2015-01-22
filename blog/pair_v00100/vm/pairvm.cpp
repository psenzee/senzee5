// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "script.h"
#include "vm/read_file.h"

#include <conio.h>
#include <new>

void __cdecl break_new_handler()
{
    _asm { int 3 }
}
/*
* fileiterator, streams, arrays/buffers and execing processes into pair
* gc into pair (now, before it's too late..), refcounted/mark&sweep hybrid
                * separate out a pairformatreader/pairformatwriter ?
                * pair2cpp?
                * unification opcode for pair?
                * a generally usable backend so that other programs can use lisp-like functionality directly..
                * so, everything must be created by a factory (or at least registered at time of creation)
                -- pair:
put in 'pair' namespace
set up vtables manually, c like
have 'type tables' (16 byte aligned) and a ttable ptr just like a vtable ptr,
except: if lower 4 bits are set, they are an index into a 16 element array containing the primitive type table ptrs.
        type tables also have more reflective structure than c++ vtables
        we must be able to serialize pert near anything
*/

extern void print_sizes();
extern void pairutil_test();

int main(int argc, const char **argv)
{
//  print_sizes();
//  pairutil_test();

    std::set_new_handler(break_new_handler);

	std::string   code;
    bool          success  = true;
	int           argi     = 1;
	bool          debug    = false,
		          listing  = false;
	const char   *opt      = NULL,
		         *filename = NULL;

    opt = argv[argi++];
    if (!opt || strlen(opt) == 0)
	    success = false;
    else if (stricmp(opt, "-DEBUG") == 0)
	{
	    debug = true;
		opt   = 0;
	    if (argi == argc) success = false;
		else              opt     = argv[argi++];
	}
    if (!opt || (code = file::read_file_string(filename = opt)).size() == 0)
		success = false;

	if (!success)
	{
		fprintf(stderr, "Usage: pairvm [-debug] <compiled-script-file>\r\n");
		return -1;
	}
    Code *c = 0;
    char message[2048];
    const char *error = 0;
    try
    {
        Script::run(code);
    }
    catch (std::exception &e)
    {
        strcpy(message, e.what());
        error = message;
    }
    /*
    catch (...)
    {
        error = "Internal Error in Script\r\n";
    }
    */
    if (error)
    {
        fprintf(stderr, "%s\r\n", error);
        return -1;
    }
    Script::destroy(c);
    return 0;
}