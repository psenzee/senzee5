// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "staticevaluator.h"
#include "pairutil.h"
#include "strings.h"

#include "arithmetic.h"
#include "reader.h"
#include "unify.h"
#include "compileexception.h"
#include "read_file.h"
#include "format.h"

#include <string.h>
#include <string>
#include <map>
#include <vector>

bool StaticEvaluator::is_identifier(Object *u)
{
    return Object::cast_symbol(u) != 0;
}

bool StaticEvaluator::is_numeric_literal(Object *u)
{
    return Object::cast_number(u) != 0;
}

bool StaticEvaluator::is_string_literal(Object *u)
{
    return Object::cast_string(u) != 0;
}

bool StaticEvaluator::is_constant_true(Object *u)
{
    Symbol *id = Object::cast_symbol(u);
    return id && stricmp(id->c_str(), "true") == 0; // $TODO, change this!
}

bool StaticEvaluator::is_constant_nil(Object *u)
{
    if (!u) return true;
    Symbol *id = Object::cast_symbol(u);
    return id && stricmp(id->c_str(), "nil") == 0; // $TODO, change this!
}

bool StaticEvaluator::is_literal(Object *u)
{
    return is_numeric_literal(u) || is_string_literal(u);
}

bool StaticEvaluator::is_quote_expression(Object *u)
{
    const char *head_id = head_identifier(u);
    return head_id && stricmp(head_id, "quote") == 0;
}

bool StaticEvaluator::is_known_true(Object *u)
{
    return is_constant_true(u) || is_quote_expression(u) || is_literal(u);
}

bool StaticEvaluator::is_known_false(Object *u)
{
    return is_constant_nil(u);
}

bool StaticEvaluator::is_unknown(Object *u)
{
    return !is_known_true(u) && !is_known_false(u);
}

const char *StaticEvaluator::head_identifier(Object *u)
{
    Pair *p = Object::cast_pair(u);
    if (!p) return 0;
    Object *v = p->head;
    if (!v) return 0;
    Symbol *id = Object::cast_symbol(v);
    return id ? id->c_str() : 0;
}

Object *StaticEvaluator::evaluate(Object *u, IEvaluator *) // ignore the root evaluator..
{
    Pair *p = 0;
    if (is_known_true(u) || is_known_false(u) || is_identifier(u))
        return u;
    else if ((p = Object::cast_pair(u)) != 0)
    {
        const char *head_id = head_identifier(p);
        if (head_id)
        {
            IEvaluator *ec = m_map->get(head_id);
            if (ec) u = ec->evaluate(p, this);
            else    u = map_evaluate(this, p);
        }
    }
    return u;
}

Pair *StaticEvaluator::map_evaluate(IEvaluator *ev, Pair *p)
{
    PairIter pi(p), e(0);
    PairList list;
    bool     first = true;
    for (; pi != e; ++pi)
    {
        Object *u = *pi;
        list.add(first ? u : ev->evaluate(u, ev)); // don't eval the first item..?
        first = false;
    }
    return list.list;
}

Pair *StaticEvaluator::remove_constants(Pair *p)
{
    PairIter pi(p), e(0);
    PairList list;
    bool     first = true;
    for (; pi != e; ++pi)
    {
        Object *u = *pi;
        if (first || is_unknown(u)) list.add(u);
        first = false;
    }
    return list.list;
}

Pair *StaticEvaluator::remove_nonfunctions(Pair *p, bool keep_last)
{
    PairIter pi(p), e(0);
    PairList list;
    bool     first = true;
    Object  *u = 0;
    for (; pi != e; ++pi)
    {
        u = *pi;
        if (first || PairUtil::cast_pair(u)) list.add(u);
        first = false;
    }
    if (keep_last && !PairUtil::cast_pair(u))
        list.add(u);
    return list.list;
}

class ListEvaluator : public IEvaluator
{
public:

    std::vector<IEvaluator *> list;

    Object *evaluate(Object *u, IEvaluator *ev)
    {
        for (std::vector<IEvaluator *>::iterator i = list.begin(), e = list.end(); i != e; ++i)
            u = (*i)->evaluate(u, ev);
        return u;
    }
    ~ListEvaluator()
    {
        for (std::vector<IEvaluator *>::const_iterator i = list.begin(), e = list.end(); i != e; ++i)
            delete *i;
    }
};

class EvalMap : public IEvaluatorMap
{
public:

    std::map<std::string, ListEvaluator *> map;

    IEvaluator *get(const char *s)        { return map[s]; }
    IEvaluator *get(const std::string &s) { return map[s]; }

    void        add(const std::string &s, IEvaluator *ev)
    { 
        if (map.find(s) == map.end())
            map[s] = new ListEvaluator;
        map[s]->list.push_back(ev);
    }

    ~EvalMap()
    {
        for (std::map<std::string, ListEvaluator *>::const_iterator i = map.begin(), e = map.end(); i != e; ++i)
            delete (*i).second;
    }
};

class ProcedureEvaluator : public IEvaluator
{
public:
    Object *evaluate(Object *u, IEvaluator *ev)
    {
        Pair *p = Object::cast_pair(u);
        if (!p) return u;
        p = StaticEvaluator::map_evaluate(ev, p);
        p = StaticEvaluator::remove_nonfunctions(p, true); // note that this is only valid if the list that procedure generates is not used. (to ensure this, make procedure return a T instead)
        return p;
    }
};

static Object *read(const char *s)
{
    Reader reader;
    return reader.read(s);
}

Object *subst(Object *e, Object *list, bool matches_only = true, bool skip_quoted = false);

static bool has_function_symbol(const Object *e, const char *fnname)
{
    const Pair   *p = Object::cast_pair(e);
    const Symbol *s = p ? Object::cast_symbol(p->head) : 0;
    return s && s->str() == fnname;
}

inline bool is_quote(Object *e)
{
    return has_function_symbol(e, "quote");
}

Pair *subst_list(Pair *p, Object *list, bool matches_only, bool skip_quoted)
{
    if (skip_quoted && is_quote(p))
        return p;
    PairList ls;    
    for (PairIter i(p), e(0); i != e; ++i)
        ls.add(subst(*i, list, matches_only, skip_quoted));
    return ls.list;
}

Object *subst(Object *e, Object *list, bool matches_only, bool skip_quoted)
{
    if (list == Types::create_true())
        return e;
    Pair *p = PairUtil::cast_pair(e);
    if (p)
        return subst_list(p, list, matches_only, skip_quoted);
    if (matches_only && !Object::cast_match(e))
        return e;
    Pair *assoc = PairUtil::findassoc(list, e);
    return assoc ? assoc->tail : e;
}

bool ends_with(const std::string &s, const char *end)
{
    if (!end || !*end) return false;
    size_t endsz = strlen(end),
           ssz   = s.size();
    if (endsz > ssz) return false;
    return stricmp(s.data() + (ssz - endsz), end) == 0;
}

inline bool is_symbol(Object *e)
{
    return !e || e == Types::create_true() || Object::cast_symbol(e);
}

inline bool is_literal(Object *e)
{
    return e == Types::create_true() || Object::cast_number(e) || Object::cast_string(e) || is_quote(e);
}

inline bool is_literal_or_symbol(Object *e)
{
    return !e || is_symbol(e) || is_literal(e);
}

bool has_side_effect(Object *e)
{
    // $todo right now this is dead simple.  Soon, we'll expand it
    return !is_literal_or_symbol(e);
}

bool is_assoc_valid(Pair *assoc)
{
    if (!assoc) return false;
    Match *key = Object::cast_match(assoc->head);
    Object *value = assoc->tail;
    if (!key) return false;
    const std::string &ks = key->str();
    if      (ends_with(ks, "-atom"))               return !value || !Object::cast_pair(value);
    else if (ends_with(ks, "-list"))               return !value || Object::cast_pair(value);
    else if (ends_with(ks, "-true-or-nil"))        return !value || value == Types::create_true();
    else if (ends_with(ks, "-nil"))                return !value;
    else if (ends_with(ks, "-true"))               return value == Types::create_true();
    else if (ends_with(ks, "-literal-or-symbol"))  return is_literal_or_symbol(value);
    else if (ends_with(ks, "-symbol"))             return is_symbol(value);
    else if (ends_with(ks, "-numeric"))            return Object::cast_number(value) != 0;
    else if (ends_with(ks, "-string"))             return Object::cast_string(value) != 0;
    else if (ends_with(ks, "-quote"))              return is_quote(value);
    else if (ends_with(ks, "-literal"))            return is_literal(value);
    else if (ends_with(ks, "-literal-or-nil"))     return !value || is_literal(value);
    else if (ends_with(ks, "-fx"))                 return  has_side_effect(value);
    else if (ends_with(ks, "-no-fx"))              return !has_side_effect(value);
    return true;
}

bool is_list_valid(Object *assoc_list)
{
    if (assoc_list == Types::create_true())
        return true;
    Pair *p = Object::cast_pair(assoc_list);
    if (!p) return false;
    // check to see if entries that are required to be certain types are
    for (PairIter i(p), e(0); i != e; ++i)
        if (!is_assoc_valid(Object::cast_pair(*i)))
            return false;
    return true;
}

class UnifyEvaluator : public IEvaluator
{
    Pair   *template_in;
    Object *template_out;
public:
    UnifyEvaluator(const char *in, const char *out)
    {
        template_in  = Object::cast_pair(read(in));
        template_out = read(out);
    }
    UnifyEvaluator(Pair *template_in, Object *template_out)
        : template_in(template_in), template_out(template_out) {}
    Object *evaluate(Object *u, IEvaluator *ev)
    {
        Pair *p = Object::cast_pair(u);
        p = StaticEvaluator::map_evaluate(ev, p);
        if (PairUtil::length(p) != PairUtil::length(template_in))
            return p;
        Object *list = Unifier::unify(template_in, p);
        if (!is_list_valid(list)) return p;
        return ev->evaluate(subst(template_out, list), ev);
    }
};

class LeftToRightNestEvaluator : public IEvaluator
{
    Symbol *m_symbol;

    Pair *nest(Pair *p)
    {
        Pair *sublist = PairUtil::list(3, m_symbol, PairUtil::at(p, 1), PairUtil::at(p, 2));
        unsigned sz = PairUtil::length(p);
        if      (sz <  3) return p; // error?
        else if (sz == 3) return sublist;
        return nest(PairUtil::pair(m_symbol, PairUtil::pair(sublist, PairUtil::nthpair(p, 3))));
    }

public:

    LeftToRightNestEvaluator(const char *symbol) : m_symbol(!symbol ? 0 : Types::create_symbol(symbol)) {}
    LeftToRightNestEvaluator(Symbol *symbol = 0) : m_symbol(symbol) {}

    Object *evaluate(Object *u, IEvaluator *ev)
    {
        Pair *p = Object::cast_pair(u);
        if (!p) return u;
        if (!m_symbol)
            m_symbol = Object::cast_symbol(p->head);
        p = nest(p);
        p = StaticEvaluator::map_evaluate(ev, p);
        return p;
    }
};

class IncludeEvaluator : public IEvaluator
{
public:
    Object *evaluate(Object *u, IEvaluator *ev)
    {
        Pair *p = Object::cast_pair(u);
        if (!p) return u;
        String *s = Object::cast_string(PairUtil::at(p, 1));
        if (!s)
            throw CompileException("No file found for inclusion.");
        const std::string &filename = s->str();
        file::buffer_t file = file::read_file(filename.c_str());
        if (!file.data || !file.size)
            throw CompileException(format::format("Unable to open file '%s' for inclusion.", filename.c_str()).c_str());
        Reader reader;
        Object *e = reader.read((const char *)file.data);
        delete [] file.data;
        return ev->evaluate(e, ev);
    }
};

unsigned symbol_id = 0;

Symbol *create_new_symbol()
{
    char buffer[16];
    sprintf(buffer, "@%u", symbol_id++);
    return Types::create_symbol(buffer);
}

class LetEvaluator : public IEvaluator
{
public:
    Object *evaluate(Object *u, IEvaluator *ev)
    {
        Pair *p = Object::cast_pair(u);
        if (!p) return u;
        Pair *assigns = Object::cast_pair(PairUtil::at(p, 1));
        Symbol *let = Types::create_symbol("$let");
        if (!assigns)
        {
            PairUtil::set(p, 0, let);
            PairUtil::set(p, 2, ev->evaluate(PairUtil::at(p, 2), ev));
            return p; // must be old-style (soon to be deprecated) let..
        }
        PairList scope, replacements;
        scope.add(Types::create_symbol("$scope"));
        for (PairIter i(assigns), e(0); i != e; ++i)
        {
            Pair   *p      = Object::cast_pair(*i), *created = 0;
            Symbol *symbol = Object::cast_symbol(p ? p->head : *i), *new_symbol = 0;
            if (!symbol)
                throw CompileException("Expected variable symbol in (let ..) expression not found!\n");
            replacements.add(PairUtil::pair(symbol, new_symbol = create_new_symbol()));
            scope.add(PairUtil::list(3, let, new_symbol, !p ? 0 : ev->evaluate(PairUtil::at(p, 1), ev)));
        }
        
        for (PairIter i(PairUtil::nthpair(p, 2)), e(0); i != e; ++i)
            scope.add(subst(ev->evaluate(*i, ev), replacements.list, false, true)); // don't recurse into quote for substitutions..
        return scope.list;
    }
};

IEvaluator *create_evaluator(Pair *p)
{
    Symbol *name = Object::cast_symbol(p->head);
    if (!name) return 0;
    const std::string &ns = name->str();
    if      (ns == "#include-evaluator")             return new IncludeEvaluator;
    else if (ns == "#left-to-right-nest-evaluator")  return new LeftToRightNestEvaluator;
    else if (ns == "#add-evaluator")                 return new AddEvaluator;
    else if (ns == "#subtract-evaluator")            return new SubtractEvaluator;
    else if (ns == "#multiply-evaluator")            return new MultiplyEvaluator;
    else if (ns == "#divide-evaluator")              return new DivideEvaluator;
    else if (ns == "#modulus-evaluator")             return new ModulusEvaluator;
    else if (ns == "#bit-or-evaluator")              return new BitOrEvaluator;
    else if (ns == "#bit-and-evaluator")             return new BitAndEvaluator;
    else if (ns == "#bit-xor-evaluator")             return new BitXorEvaluator;
    else if (ns == "#bit-shift-left-evaluator")      return new BitShiftLeftEvaluator;
    else if (ns == "#bit-shift-right-evaluator")     return new BitShiftRightEvaluator;
    else if (ns == "#let-evaluator")                 return new LetEvaluator;
    else if (ns == "#procedure-evaluator")           return new ProcedureEvaluator;
    else if (ns == "#unify")
    {
        Pair   *template_in  = Object::cast_pair(PairUtil::at(p, 1));
        Object *template_out = PairUtil::at(p, 2);
        return !template_in || !template_out ? 0 : new UnifyEvaluator(template_in, template_out);
    }
    return 0;
}

bool read(EvalMap &map, Pair *p)
{
    Symbol *name = Object::cast_symbol(p->head);
    if (!name) return false;
    printf("-- reading rule for '%s'\n", name->c_str());
    p = Object::cast_pair(p->tail);
    if (!p) return false;
    IEvaluator *ev = create_evaluator(p);
    if (!ev) return false;
    map.add(name->c_str(), ev);
    return true;
}

Object *StaticEvaluator::transform(Object *u)
{
    EvalMap map;

    const char *filename = "evaluations";
    file::buffer_t file = file::read_file(filename);
    if (!file.data || !file.size)
    {
        fprintf(stderr, "Can't find evaluations file %s or file is empty.\n", filename);
        exit(-1);
        return u;
    }
    Reader reader;
    Object *e = reader.read((const char *)file.data);
    if (!has_function_symbol(e, "evaluations"))
    {
        fprintf(stderr, "Expected (evaluations..) at beginning of evaluations file.\n");
        exit(-1);
        return u;
    }
    Pair *p = Object::cast_pair(e);
    p = Object::cast_pair(p->tail);
    if (!p)
        return u;    
    for (PairIter i(p), e(0); i != e; ++i)
    {
        Pair *q = Object::cast_pair(*i);
        if (q) read(map, q);
    }

/*
    map.add("include",    new IncludeEvaluator);

    map.add("+",          new LeftToRightNestEvaluator);
    map.add("+",          new AddEvaluator);
    map.add("-",          new LeftToRightNestEvaluator);
    map.add("-",          new SubtractEvaluator);
    map.add("*",          new LeftToRightNestEvaluator);
    map.add("*",          new MultiplyEvaluator);
    map.add("/",          new LeftToRightNestEvaluator);
    map.add("/",          new DivideEvaluator);

    map.add("%",          new LeftToRightNestEvaluator);
    map.add("%",          new ModulusEvaluator);

    map.add("|",          new LeftToRightNestEvaluator);
    map.add("|",          new BitOrEvaluator);
    map.add("&",          new LeftToRightNestEvaluator);
    map.add("&",          new BitAndEvaluator);
    map.add("^",          new LeftToRightNestEvaluator);
    map.add("^",          new BitXorEvaluator);

    map.add(">>",         new BitShiftRightEvaluator);
    map.add("<<",         new BitShiftLeftEvaluator);

    map.add("let",        new LetEvaluator);

    map.add("procedure",  new ProcedureEvaluator);
    map.add("$scope",     new ProcedureEvaluator);

    map.add("set",        new UnifyEvaluator("(set (nth ?x ?y) ?z)",            "(set-nth ?x ?y ?z)"));
    map.add("set",        new UnifyEvaluator("(set (first ?x) ?y)",             "(set-first ?x ?y)"));
    map.add("set",        new UnifyEvaluator("(set (rest ?x) ?y)",              "(set-rest ?x ?y)"));

    map.add("and",        new LeftToRightNestEvaluator);
    map.add("or",         new LeftToRightNestEvaluator);

    map.add("and",        new UnifyEvaluator("(and ?x ?y)",                     "(if ?x ?y nil)"));
    map.add("or",         new UnifyEvaluator("(or ?x-atom ?y)",                 "(if ?x-atom ?x-atom ?y)"));
    map.add("or",         new UnifyEvaluator("(or ?x ?y)",                      "(if ?x true ?y)")); // quick fix could use (if ?x true ?y) (but now [and] and [or] are not symmetric but..
//                                                                              "(if ($let @tmp1 ?x) @tmp1 ?y)" // eventually, to something like this

    map.add("true?",      new UnifyEvaluator("(true? ?x)",                      "(not-nil? ?x)"));
    map.add("nil?",       new UnifyEvaluator("(nil? ?x)",                       "(not ?x)"));
    map.add("not",        new UnifyEvaluator("(not (not ?x))",                  "(not-nil? ?x)"));
    map.add("not-nil?",   new UnifyEvaluator("(not-nil? (not ?x))",             "(not ?x)"));

    map.add("if",         new UnifyEvaluator("(if (not ?x) ?y ?z)",             "(if ?x ?z ?y)"));
    map.add("if",         new UnifyEvaluator("(if (not-nil? ?x) ?y ?z)",        "(if ?x ?y ?z)"));

    map.add("if",         new UnifyEvaluator("(if ?-true ?y ?-nil)",            "?y"));
    map.add("if",         new UnifyEvaluator("(if ?-nil ?-nil ?x)",             "?x"));
    map.add("if",         new UnifyEvaluator("(if ?x ?-nil ?-true)",            "(not ?x)"));
    map.add("if",         new UnifyEvaluator("(if ?x ?-true ?-nil)",            "(not-nil? ?x)"));
    map.add("if",         new UnifyEvaluator("(if ?x ?y-no-fx ?z-no-fx)",       "($cond ?x ?y-no-fx ?z-no-fx)"));

    map.add("$cond",      new UnifyEvaluator("($cond (not ?x) ?y ?z)",          "($cond ?x ?z ?y)"));
    map.add("$cond",      new UnifyEvaluator("($cond (not-nil? ?x) ?y ?z)",     "($cond ?x ?y ?z)"));

    map.add("$cond",      new UnifyEvaluator("($cond ?-literal ?x ?y)",         "?x"));
    map.add("$cond",      new UnifyEvaluator("($cond ?-true ?x ?-nil)",         "?x"));
    map.add("$cond",      new UnifyEvaluator("($cond ?-nil ?-nil ?x)",          "?x"));
    map.add("$cond",      new UnifyEvaluator("($cond ?x-atom ?x-atom ?-nil)",   "?x-atom"));
//  map.add("$cond",      new UnifyEvaluator("($cond ?x-atom ?-nil ?x-atom)",   "nil")); // nil doesn't work as a return value, need a stand-in, or slightly different architecture..
    map.add("$cond",      new UnifyEvaluator("($cond ?x-atom ?x-atom ?x-atom)", "?x-atom"));
*/
    StaticEvaluator se(&map);
    return se.evaluate(u, &se);
}