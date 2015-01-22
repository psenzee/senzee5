// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "vm.h"
#include "symbol.h"
#include "package.h"
#include "opcode.h"
#include "code.h"
#include "context.h"
#include "frame.h"
#include "continuation.h"
#include "exceptions.h"
#include "constantpool.h"
#include "streams.h"
#include "config.h"
#include "ffifunction.h"
#include "iterator.h"
#include "castutil.h"
#include "format.h"
#include "pairutil.h"
#include "util.h"
#include "types.h"
#include "array.h"
#include "array_t.h"

#include <math.h>

static unsigned _opcode_flags[256];
static bool     _opcode_flags_initialized = false;

enum OpcodeFlag { OCF_U = 32, OCF_V = 64, OCF_PUSH = 128,
                  OCF_OBJ = 1, OCF_INT = 2, OCF_UINT = 5, 
                  OCF_TYPE_MASK = 7 };

static void initialize_opcode_flags()
{
    if (_opcode_flags_initialized)
        return;

    memset(_opcode_flags, 0, sizeof(_opcode_flags));
    
    _opcode_flags[Opcode::OC_CALL4]   = OCF_U | OCF_INT;
    _opcode_flags[Opcode::OC_POPN4]   = OCF_INT;
    _opcode_flags[Opcode::OC_LINE4]   = OCF_INT;
    _opcode_flags[Opcode::OC_JT4]     = OCF_U | OCF_INT;
    _opcode_flags[Opcode::OC_JN4]     = OCF_U | OCF_INT;
    _opcode_flags[Opcode::OC_LIST4]   = OCF_INT | OCF_PUSH;
    _opcode_flags[Opcode::OC_APPEND4] = OCF_INT | OCF_PUSH;
    _opcode_flags[Opcode::OC_ARRAY4]  = OCF_INT | OCF_PUSH;
    _opcode_flags[Opcode::OC_JMP4]    = OCF_INT;
    _opcode_flags[Opcode::OC_PROC4]   = OCF_INT;

    _opcode_flags[Opcode::OC_POP]     = OCF_U;
    _opcode_flags[Opcode::OC_DUP]     = OCF_PUSH;

    _opcode_flags[Opcode::OC_PAIR]    = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_HEAD]    = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_TAIL]    = OCF_U | OCF_PUSH;

    _opcode_flags[Opcode::OC_LOCAL4]  = OCF_UINT   | OCF_PUSH;

    _opcode_flags[Opcode::OC_LOCAL_0] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_1] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_2] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_3] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_4] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_5] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_6] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_7] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_8] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_9] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_10] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_11] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_12] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_13] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_14] = OCF_PUSH;
    _opcode_flags[Opcode::OC_LOCAL_15] = OCF_PUSH;

    _opcode_flags[Opcode::OC_APPLY]   = OCF_U | OCF_V;
    _opcode_flags[Opcode::OC_SETL4]    = OCF_U | OCF_UINT;

    _opcode_flags[Opcode::OC_SETL_0]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_1]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_2]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_3]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_4]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_5]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_6]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_7]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_8]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_9]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_10]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_11]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_12]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_13]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_14]   = OCF_U;
    _opcode_flags[Opcode::OC_SETL_15]   = OCF_U;

    _opcode_flags[Opcode::OC_INT4]    = OCF_INT    | OCF_PUSH;

    _opcode_flags[Opcode::OC_LINK_CONST]  = OCF_OBJ | OCF_PUSH;
    _opcode_flags[Opcode::OC_LINK_FUNC]   = OCF_OBJ | OCF_PUSH;
    _opcode_flags[Opcode::OC_LINK_GLOBAL] = OCF_OBJ | OCF_PUSH;
    _opcode_flags[Opcode::OC_LINK_SETG]   = OCF_U | OCF_OBJ;

    _opcode_flags[Opcode::OC_ARGS]    = OCF_PUSH;
    _opcode_flags[Opcode::OC_NOT]     = OCF_PUSH;

    _opcode_flags[Opcode::OC_TRUE]    = OCF_PUSH;
    _opcode_flags[Opcode::OC_NIL]     = OCF_PUSH;

    _opcode_flags[Opcode::OC_TRUE]    = OCF_PUSH;
    _opcode_flags[Opcode::OC_NIL]     = OCF_PUSH;

    _opcode_flags[Opcode::OC_ADD]     = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_SUB]     = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_MUL]     = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_DIV]     = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_MOD]     = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_SHR]     = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_SHL]     = OCF_U | OCF_V | OCF_PUSH;

    _opcode_flags[Opcode::OC_BITAND]  = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_BITOR]   = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_BITXOR]  = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_BITNOT]  = OCF_U | OCF_PUSH;

    _opcode_flags[Opcode::OC_LT]      = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_LTE]     = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_GT]      = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_GTE]     = OCF_U | OCF_V | OCF_PUSH;

    _opcode_flags[Opcode::OC_EQ]      = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_EQUAL]   = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_EQL]     = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_OFTYPEP] = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_COND]    = OCF_U | OCF_V | OCF_PUSH;

    _opcode_flags[Opcode::OC_NEXTP]   = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_NEXT]    = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_ITER]    = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_CPLIST]  = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_CPTREE]  = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_LENGTH]  = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_NOT]     = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_NOTNILP] = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_NTH]     = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_SETNTH]  = OCF_U | OCF_V | OCF_PUSH;
    _opcode_flags[Opcode::OC_TYPE]    = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_ASSOCP]  = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_ATOMP]   = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_PAIRP]   = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_INTP]    = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_ITERP]   = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_NUMP]    = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_REALP]   = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_STRINGP] = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_SYMBOLP] = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_USERP]   = OCF_U | OCF_PUSH;
    _opcode_flags[Opcode::OC_TYPEP]   = OCF_U | OCF_PUSH;
}

unsigned Vm::INSTRUCTION_COUNT = 0;

static inline Object     *m_peek(std::vector<Object *> &stack)                     { return stack.back(); }
static inline Object     *m_pop (std::vector<Object *> &stack)                     { Object *r = stack.back(); stack.pop_back(); return r; }
static inline void        m_pop (std::vector<Object *> &stack, size_t count)       { stack.resize(stack.size() - count); }
static inline void        m_push(std::vector<Object *> &stack, Object *r)          { stack.push_back(r); }

static inline Collection *m_collection(Object *r)                                  { return r ? r->cast_collection() : 0; }
static inline Iterable   *m_iterable(Object *r)                                    { return r ? r->cast_iterable() : 0; }
static inline Number     *m_num(Object *r)                                         { return CastUtil::number(r); }
static inline Iterator   *m_iterator(Object *r)                                    { Iterable *i = m_iterable(r); return i ? i->iterator() : &NullIterator::instance; }
static inline Iterator   *m_cast_iter(Object *r)                                   { return r ? r->cast_iterator() : &NullIterator::instance; }
static inline Object     *m_get(Object *r, unsigned n)                             { Collection *c = m_collection(r); return c ? c->get(n) : 0; }
static inline void        m_set(Object *r, unsigned n, Object *e)                  { Collection *c = m_collection(r); if (c) c->set(n, e); }
static inline int         m_length(Object *r)                                      { Collection *c = m_collection(r); return c ? (int)c->length() : -1; }

static inline Object     *m_istype(Object *u, const Type &type)                    { return u && u->type().of_type(type) ? (Object *)&u->type() : 0; }

static inline int         m_integer(Object *e)                                     { return (int)*m_num(e); }
static inline float       m_float(Object *e)                                       { return (float)*m_num(e); }
static inline Object     *m_tf(bool b)                                             { return b ? Vm::VM_TRUE : 0; }

typedef int   (*binary_opi_t)(int, int);
typedef float (*binary_opf_t)(float, float);
typedef int   (*unary_opi_t)(int);
typedef float (*unary_opf_t)(float);

template <typename T> static T _add (T x, T y) { return x + y; }
template <typename T> static T _sub (T x, T y) { return x - y; }
template <typename T> static T _mul (T x, T y) { return x * y; }
template <typename T> static T _div (T x, T y) { return x / y; }
template <typename T> static T _mod (T x, T y) { return x % y; }
template <typename T> static T _modf(T x, T y) { return fmodf(x, y); }
template <typename T> static T _or  (T x, T y) { return x | y; }
template <typename T> static T _and (T x, T y) { return x & y; }
template <typename T> static T _xor (T x, T y) { return x ^ y; }
template <typename T> static T _shr (T x, T y) { return (T)((int)x >> y); }
template <typename T> static T _slr (T x, T y) { return (T)((unsigned)x >> y); }
template <typename T> static T _shl (T x, T y) { return x << y; }
template <typename T> static T _not (T x)      { return ~x; }

inline static Number *m_binary_opi(Number *u, Number *v, binary_opi_t opi) { return Types::create_int(opi((int)*u, (int)*v)); }
inline static Number *m_binary_opf(Number *u, Number *v, binary_opf_t opf) { return Types::create_float(opf((float)*u, (float)*v)); }
inline static Number *m_unary_opi(Number *u, unary_opi_t opi)              { return Types::create_int(opi((int)*u)); }
inline static Number *m_unary_opf(Number *u, unary_opf_t opf)              { return Types::create_float(opf((float)*u)); }

static Object        *m_binary_opi(Object *u, Object *v, binary_opi_t opi) { return m_binary_opi(m_num(u), m_num(v), opi); }
static Object        *m_binary_opf(Object *u, Object *v, binary_opf_t opf) { return m_binary_opf(m_num(u), m_num(v), opf); }
static Object        *m_unary_opi(Object *u, unary_opi_t opi)              { return m_unary_opi(m_num(u), opi); }
static Object        *m_unary_opf(Object *u, unary_opf_t opf)              { return m_unary_opf(m_num(u), opf); }

static Object *m_binary_opfi(Object *u, Object *v, binary_opi_t opi, binary_opf_t opf)
{
    Number *nu = m_num(u), *nv = m_num(v);
    // there must be a faster/better way to check this..
    return (nu->isinteger() && nv->isinteger()) ? m_binary_opi(nu, nv, opi) : m_binary_opf(nu, nv, opf);
}

static Object *m_unary_opfi(Object *u, unary_opi_t opi, unary_opf_t opf)
{
    Number *nu = m_num(u);
    // there must be a faster/better way to check this..
    return nu->isinteger() ? m_unary_opi(nu, opi) : m_unary_opf(nu, opf);
}

static bool m_oftypep(Object *u, Object *v)
{
    u = cast_to_type(u, type_type); v = cast_to_type(v, type_type);
    return (((const Type *)u)->of_type(*(const Type *)v));
}

static int m_compare(Object *u, Object *v)
{
    if (u == v) return  0;
    if (!u)     return -1; // NIL policy: always less than..
    if (!v)     return  1;
    if (!u->comparable_to(v))
        throw RuntimeException(format::format("unable to compare instances of types '%s' and '%s'\n", u->type().type_name, v->type().type_name).c_str());
    int c = u->compare_to(v);
    return c;
}

static Object *m_append(std::vector<Object *> &stack, int count)
{
    if (count < 1)
        return 0;
    Object *first = m_pop(stack), *u = first;
    // $TODO should do this in a more efficient way for > 2 args!!!
    // use PairList?
    for (int i = 1; i < count; i++)
    {
        Object *v = m_pop(stack);
        u = PairUtil::append(u, v);
    }
    return u;
}

static Object **m_reverse_stack_top(std::vector<Object *> &stack, unsigned count)
{
    Object **s = &stack[stack.size() - count];
    array_t list;
    list.last = list.first = (void **)s; list.last += count;
    reverse(list);
    return s;
}

static Object *m_list(std::vector<Object *> &stack, int count)
{
    Object *u = PairUtil::list(m_reverse_stack_top(stack, count), count);
    stack.resize(stack.size() - count);
    return u;
}

static Object *m_array(std::vector<Object *> &stack, int count)
{
    Array *u = new Array(m_reverse_stack_top(stack, count), count);
    stack.resize(stack.size() - count);
    return u;
}

static void m_apply(Object *u, Object *v, Context *context)
{
    u = cast_to_type(u, type_function);
    v = cast_to_type(v, type_pair);
    context->apply((Function *)u, v);
}

bool Vm::execute(Context *context, IGarbageCollector *gc)
{
    initialize_opcode_flags();

   Code                  *program   = context->program;
   ConstantPool          *pool      = program->pool;
   Frame                 *frame     = 0;
   const unsigned char   *code      = 0;
   std::vector<Object *> &stack     = context->stack;

   unsigned               flags     = 0,
                          uv        = 0,
                          type      = 0;
   int                    count     = 0,
                          param     = 0,
                          ip        = 0,
                          line      = 0,
                          i         = 0,
                          sz        = 0;
   Function              *fun       = 0;
   unsigned               oc        = 0;
   Object                *u         = 0,
                         *v         = 0,
                         *result    = 0;
   void                  *obj       = 0;

   if (context->frames.empty())
       return false; // if there's no frame, we're done

   while (true)
   {
       frame = context->frame();
       code  = frame->code;
       ip    = frame->ip;
       oc    = frame->nextubyte();
       flags = _opcode_flags[oc];
       uv    = flags & (OCF_U | OCF_V);
       type  = flags & OCF_TYPE_MASK;

       if (!(++INSTRUCTION_COUNT & 1023) && gc) gc->gc();

       if (uv)
       {
           if (uv & OCF_U) u = m_pop(stack);
           if (uv & OCF_V) v = m_pop(stack);
       }

       if (type)
       {
           switch (type)
           {
           case OCF_OBJ:    obj   = frame->nextobject(); break;
           case OCF_INT:    param = frame->nextint();    break;
           case OCF_UINT:   param = frame->nextuint();   break;
           }
       }

       switch (oc)
       {
       case Opcode::OC_LINE4:      line = param; break;

       case Opcode::OC_NOP:
       case Opcode::OC_TRACE:
       case Opcode::OC_PROC4:
       case Opcode::OC_POP:
       case Opcode::OC_BREAK:      break;

       case Opcode::OC_DUP:        result = m_peek(stack); break;

       case Opcode::OC_POPN4:      m_pop(stack, param); break;

       case Opcode::OC_PAIR:       result = PairUtil::pair(u, v); break;

       case Opcode::OC_HEAD:       result = PairUtil::head(u); break;
       case Opcode::OC_TAIL:       result = PairUtil::tail(u); break;

       case Opcode::OC_LOCAL_0:  case Opcode::OC_LOCAL_1:  case Opcode::OC_LOCAL_2:  case Opcode::OC_LOCAL_3:
       case Opcode::OC_LOCAL_4:  case Opcode::OC_LOCAL_5:  case Opcode::OC_LOCAL_6:  case Opcode::OC_LOCAL_7:
       case Opcode::OC_LOCAL_8:  case Opcode::OC_LOCAL_9:  case Opcode::OC_LOCAL_10: case Opcode::OC_LOCAL_11:
       case Opcode::OC_LOCAL_12: case Opcode::OC_LOCAL_13: case Opcode::OC_LOCAL_14: case Opcode::OC_LOCAL_15:

                                    param = oc - Opcode::OC_LOCAL_0; // fall-through

       case Opcode::OC_LOCAL4:      result = frame->local(param); break;

       case Opcode::OC_SETL_0:   case Opcode::OC_SETL_1:   case Opcode::OC_SETL_2:   case Opcode::OC_SETL_3:
       case Opcode::OC_SETL_4:   case Opcode::OC_SETL_5:   case Opcode::OC_SETL_6:   case Opcode::OC_SETL_7:
       case Opcode::OC_SETL_8:   case Opcode::OC_SETL_9:   case Opcode::OC_SETL_10:  case Opcode::OC_SETL_11:
       case Opcode::OC_SETL_12:  case Opcode::OC_SETL_13:  case Opcode::OC_SETL_14:  case Opcode::OC_SETL_15:

                                    param = oc - Opcode::OC_SETL_0; // fall-through

       case Opcode::OC_SETL4:       frame->setlocal(param, u); break;

       case Opcode::OC_INT4:        result = Types::create_int(param); break;

       case Opcode::OC_LINK_FUNC:
       case Opcode::OC_LINK_CONST:  result = (Object *)obj;   break;
       case Opcode::OC_LINK_GLOBAL: result = *(Object **)obj; break;
       case Opcode::OC_LINK_SETG:   *(Object **)obj = u;      break;

       case Opcode::OC_JT4:         frame->ip +=  u ? param : 0; break;
       case Opcode::OC_JN4:         frame->ip += !u ? param : 0; break;
       case Opcode::OC_JMP4:        frame->ip += param; break;

       case Opcode::OC_CALL4:       context->call(Object::cast_function(u), param); break;
       case Opcode::OC_RET:         context->doreturn(); if (context->frames.empty()) return false; else break; // if the frame stack is empty, we're done..

       case Opcode::OC_TRUE:        result = VM_TRUE; break;
       case Opcode::OC_NIL:         result = 0;       break;

       case Opcode::OC_CONTIN: 
       case Opcode::OC_RESUME:      throw RuntimeException("Continuations are currently unsupported.\n");

       case Opcode::OC_YIELD:       return true;
       case Opcode::OC_END:         context->clear(); return false;

       case Opcode::OC_ADD:         result = m_binary_opfi(u, v, _add<int>, _add<float>);  break;
       case Opcode::OC_SUB:         result = m_binary_opfi(u, v, _sub<int>, _sub<float>);  break;
       case Opcode::OC_MUL:         result = m_binary_opfi(u, v, _mul<int>, _mul<float>);  break;
       case Opcode::OC_DIV:         result = m_binary_opfi(u, v, _div<int>, _div<float>);  break;
       case Opcode::OC_MOD:         result = m_binary_opfi(u, v, _mod<int>, _modf<float>); break;

       case Opcode::OC_SHR:         result = m_binary_opi (u, v, _shr<int>);               break;
       case Opcode::OC_SHL:         result = m_binary_opi (u, v, _shl<int>);               break;
       case Opcode::OC_BITAND:      result = m_binary_opi (u, v, _and<int>);               break;
       case Opcode::OC_BITOR:       result = m_binary_opi (u, v, _or <int>);               break;
       case Opcode::OC_BITXOR:      result = m_binary_opi (u, v, _xor<int>);               break;

       case Opcode::OC_BITNOT:      result = m_unary_opi  (u,    _not<int>);               break;

       case Opcode::OC_LT:          result = m_tf(m_compare(u, v) <  0); break;
       case Opcode::OC_LTE:         result = m_tf(m_compare(u, v) <= 0); break;
       case Opcode::OC_GT:          result = m_tf(m_compare(u, v) >  0); break;
       case Opcode::OC_GTE:         result = m_tf(m_compare(u, v) >= 0); break;

       case Opcode::OC_EQ:          result = m_tf(u == v); break; 
       case Opcode::OC_EQUAL:  
       case Opcode::OC_EQL:         result = m_tf(m_compare(u, v) == 0); break; 

       case Opcode::OC_NEXTP:       result = m_tf(m_cast_iter(u)->hasnext()); break;
       case Opcode::OC_NEXT:        result = m_cast_iter(u)->next(); break;
       case Opcode::OC_ITER:        result = m_iterator(u); break;

       case Opcode::OC_APPEND4:     result = m_append(stack, param); break;

       case Opcode::OC_CPLIST:      result = PairUtil::copylist(u); break;
       case Opcode::OC_CPTREE:      result = PairUtil::copytree(u); break;

       case Opcode::OC_LIST4:       result = m_list (stack, param); break;
       case Opcode::OC_ARRAY4:      result = m_array(stack, param); break;

       case Opcode::OC_APPLY:       m_apply(u, v, context); break;

       case Opcode::OC_ARGS:        result = frame->args(); break;

       case Opcode::OC_LENGTH:      result = (i = u ? m_length(u) : -1) == -1 ? 0 : Types::create_int(i); break;

       case Opcode::OC_NOT:         result = m_tf(!u); break;
       case Opcode::OC_NOTNILP:     result = m_tf(u != 0); break;
       case Opcode::OC_COND:        result = (m_pop(stack) != 0) ? u : v; break;

       case Opcode::OC_NTH:         result = m_get(u, m_integer(v)); break;
       case Opcode::OC_SETNTH:      result = m_pop(stack); m_set(u, m_integer(v), result); break;
       case Opcode::OC_TYPE:        result = !u ? 0 : (Object *)&u->type(); break;

       case Opcode::OC_ASSOCP:      result = m_tf(PairUtil::isassoc(u)); break;
       case Opcode::OC_ATOMP:       result = m_tf(PairUtil::isatom (u)); break;
       case Opcode::OC_PAIRP:       result = m_tf(PairUtil::ispair (u)); break;

       case Opcode::OC_INTP:        result = m_istype(u, type_integer);    break;
       case Opcode::OC_ITERP:       result = m_istype(u, type_iterator);   break;
       case Opcode::OC_NUMP:        result = m_istype(u, type_number);     break;
       case Opcode::OC_REALP:       result = m_istype(u, type_real);       break;
       case Opcode::OC_STRINGP:     result = m_istype(u, type_string);     break;
       case Opcode::OC_SYMBOLP:     result = m_istype(u, type_symbol);     break;
       case Opcode::OC_USERP:       result = m_istype(u, type_pointer);    break;
       case Opcode::OC_TYPEP:       result = m_istype(u, type_type);       break;
       case Opcode::OC_OFTYPEP:     result = m_tf(m_oftypep(u, v));        break;

       default:                     throw RuntimeException(format::format("Unknown opcode (%d) at %d", oc, ip).c_str()); return false;
       }

       if (flags & OCF_PUSH)
           m_push(stack, result);
   }
}

class True : public Symbol
{
public:
    inline True()  : Symbol("true") { ICollectable::created--; }
    inline ~True() { ICollectable::destroyed--; }
};

Object *Vm::VM_TRUE = new True;