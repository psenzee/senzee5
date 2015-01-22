// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "reader_test.h"

#include "pairutil.h"
#include "object.h"
#include "reader.h"
#include "reader_test_data.h"
#include "unify.h"
#include "format.h"

#include "test_utils.h"

#include <map>

#include "compiler/scriptcompiler.h"
#include "compiler/code.h"
#include "ievaluator.h"
#include "compiler/staticevaluator.h"
#include "disassembler/disassembler.h"

void test_unify(const char *pattern, const char *data)
{
    Reader reader;

    Object *p = reader.read(pattern),
           *d = reader.read(data);

    printf("Unifying ");
    print_console(p);
    printf(" and ");
    print_console(d);
    printf(":\n");
    printf("Result:\n");
    Object *u = Unifier::unify(p, d);
    if (!u) printf("Unification failed!\n");
    else    println_console(u);
}

void unifier_test()
{
    test_unify("(?x + 1)", "(2 + ?y)");
    test_unify("(?x)", "(?y)");
    test_unify("(?x ?x)", "(?y ?y)");
    test_unify("(?x ?x ?x)", "(?y ?y ?y)");
    test_unify("(?x ?y)", "(?y ?x)");
    test_unify("'a", "'a");
    test_unify("'a", "'b");
    test_unify("'a", "(?t a)");
    test_unify("(and ?x ?y)", "(and a b)");
//  test_unify("(?x ?y a)", "(?y ?x ?x)"); // fails occurs check?
    test_unify("(?x ?y ?z)", "((?y ?z) (?x ?z) (?x ?y))"); // should fail occurs check?
    test_unify("(a b c ?x)", "(?z b ?y d)");
    wait_console();
}

void compile_test(const char *filename, const char *text)
{
    Reader reader;

    Object *e = reader.read(text);

    ScriptCompiler sc(Object::cast_pair(e), true);

    Code *c = sc.compile_code();

    FileOutStream out(filename);
    c->write(out);
    out.close();

    std::string s = Disassembler::disassemble(c);
    
    FileOutStream listing(format::format("%s.lst", filename).c_str());
    listing.write(s.data(), s.size());
    listing.close();

    printf("compiling complete\n");
    wait_console();
}

void reader_test()
{
    Reader reader;

    Object *e = reader.read(program);

    println_console(e);
    wait_console();
    clear_console(5);

    println_console(reader.read("((x . (a b c (d . e) f)) . y)"));
    println_console(reader.read("(rose . (violet))"));
    println_console(reader.read("(1 . (2 . (3 . (4 . nil))))"));

    wait_console();
    clear_console(5);

    unifier_test();

    compile_test("compiled.bin", basic0);
/*
    eval_test("(+ 2 (+ 3 (+ 8 (- 12 32))))");
    eval_test("(+ 2 (+ (+ 8 (- 12 32)) (* 2 (/ 3 (% 8 (+ 12 32)))))");

    eval_test("(procedure true nil (if (of-type? bob) true nil) (if (and a b) (print hi) (print bye)) (if true true nil) true)");
*/
}