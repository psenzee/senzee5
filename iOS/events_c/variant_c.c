#include "variant_c.h"

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

const char *Variant_TypeString(VariantType type)
{
    switch (type)
    {
    case VariantType_Null:     return "<null>";
    case VariantType_Boolean:  return "<boolean>";
    case VariantType_Integer:  return "<integer>";
    case VariantType_Real:     return "<real>";
    case VariantType_Ptr:      return "<ptr>";
    case VariantType_ConstPtr: return "<const_ptr>";
    case VariantType_ObjCId:   return "<objc>";
    case VariantType_String:   return "<string>";
    case VariantType_Array:    return "<array>";
    default: break;
    }
    return "<unknown>";
}

const char *Variant_StringDescription(const Variant *v)
{
    if (!v)
        return "NULL";
    static char buffer[256];
    char strbuf[128];
    memset(strbuf, 0, sizeof(strbuf));
    if (v->type == VariantType_String)
        strncpy(strbuf, 127, v->string);
    const char *typestr = Variant_TypeString(v->type);
    switch (v->type)
    {
    case VariantType_Boolean:  return v->boolean ? "<boolean:true>" : "<boolean:false>";
    case VariantType_Integer:  sprintf(buffer, "<integer:%d>", (int)v->integer);     break;
    case VariantType_Real:     sprintf(buffer, "<real:%.2f>", v->real);         break;
    case VariantType_Ptr:      sprintf(buffer, "<ptr:%p>", v->ptr);             break;
    case VariantType_ConstPtr: sprintf(buffer, "<const_ptr:%p>", v->const_ptr); break;
    case VariantType_ObjCId:   sprintf(buffer, "<objc:%p>", v->objc_id);        break;
    case VariantType_String:   sprintf(buffer, "<string:%s>", strbuf);         break;
    case VariantType_Array:    sprintf(buffer, "<array:#%d>", v->array_count);   break;
    default: return typestr;
    }
    return buffer;
}

int Variant_IsA(const Variant *v, VariantType type)
{
    return v ? (type == v->type) : type == VariantType_Null;
}

int Variant_IsNull(const Variant *v)              { return Variant_IsA(v, VariantType_Null); }
int Variant_IsInteger(const Variant *v)           { return Variant_IsA(v, VariantType_Integer); }
int Variant_IsBoolean(const Variant *v)           { return Variant_IsA(v, VariantType_Boolean); }
int Variant_IsReal(const Variant *v)              { return Variant_IsA(v, VariantType_Real); }
int Variant_IsPtr(const Variant *v)               { return Variant_IsA(v, VariantType_Ptr); }
int Variant_IsConstPtr(const Variant *v)          { return Variant_IsA(v, VariantType_ConstPtr); }
int Variant_IsObjCId(const Variant *v)            { return Variant_IsA(v, VariantType_ObjCId); }
int Variant_IsString(const Variant *v)            { return Variant_IsA(v, VariantType_String); }
int Variant_IsArray(const Variant *v)             { return Variant_IsA(v, VariantType_Array); }

int Variant_IsAnyPrimitive(const Variant *v)
{ 
    return v && (v->type == VariantType_Integer ||
                 v->type == VariantType_Boolean ||
                 v->type == VariantType_Real);
}

int Variant_IsAnyPointer(const Variant *v)
{ 
    return v && (v->type == VariantType_Ptr      || 
                 v->type == VariantType_ConstPtr || 
                 v->type == VariantType_ObjCId   || 
                 v->type == VariantType_String); 
}

int Variant_IsArrayOrAnyPointer(const Variant *v)
{ 
    return v && (v->type == VariantType_Ptr      || 
                 v->type == VariantType_ConstPtr || 
                 v->type == VariantType_ObjCId   || 
                 v->type == VariantType_String   ||
                 v->type == VariantType_Array); 
}

Variant Variant_Null(void)
{
    Variant v;
    memset(&v, 0, sizeof(Variant));
    v.type = VariantType_Null;
    return v;
}
Variant Variant_True(void)    { return Variant_Boolean(1); }
Variant Variant_False(void)   { return Variant_Boolean(0); }

Variant Variant_Integer(int64_t value)
{
    Variant v;
    memset(&v, 0, sizeof(Variant));
    v.type = VariantType_Integer;
    v.integer = value;
    return v;
}

Variant Variant_Boolean(int value)
{
    Variant v;
    memset(&v, 0, sizeof(Variant));
    v.type = VariantType_Boolean;
    v.boolean = value;
    return v;
}

Variant Variant_Real(double value)
{
    Variant v;
    memset(&v, 0, sizeof(Variant));
    v.type = VariantType_Real;
    v.real = value;
    return v;
}

Variant Variant_Ptr(void *value)
{
    Variant v;
    memset(&v, 0, sizeof(Variant));
    v.type = VariantType_Ptr;
    v.ptr = value;
    return v;
}

Variant Variant_ConstPtr(const void *value)
{
    Variant v;
    memset(&v, 0, sizeof(Variant));
    v.type = VariantType_ConstPtr;
    v.const_ptr = value;
    return v;
}

Variant Variant_ObjCId(void *value)
{
    Variant v;
    memset(&v, 0, sizeof(Variant));
    v.type = VariantType_ObjCId;
    v.objc_id = value;
    return v;
}

Variant Variant_String(const char *value)
{
    Variant v;
    memset(&v, 0, sizeof(Variant));
    v.type = VariantType_String;
    v.string = value;
    return v;
}

Variant Variant_Array(Variant *array, int count)
{
    Variant v;
    memset(&v, 0, sizeof(Variant));
    v.type = VariantType_Array;
    v.array = array;
    v.array_count = count;
    return v;
}