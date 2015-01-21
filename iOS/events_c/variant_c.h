#ifndef _VARIANT_C_H
#define _VARIANT_C_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long long vint64_t;
    
typedef enum
{
    VariantType_Null = 0,
    VariantType_Boolean,
    VariantType_Integer,
    VariantType_Real,
    VariantType_Ptr,
    VariantType_ConstPtr,    
    VariantType_ObjCId,
    VariantType_String,
    VariantType_Array,    
    
    VariantType_MAX_TYPES
    
} VariantType;

typedef struct Variant_t
{
    VariantType            type;
    union
    {
        vint64_t           integer;
        int                boolean;
        double             real;
        void              *ptr;
        const void        *const_ptr;
        void              *objc_id;
        const char        *string;
        struct Variant_t  *array;
    };
    int                    array_count;
    
}  Variant;

int Variant_IsA(const Variant *v, VariantType type);
int Variant_IsNull(const Variant *v);
int Variant_IsInteger(const Variant *v);
int Variant_IsBoolean(const Variant *v);
int Variant_IsReal(const Variant *v);
int Variant_IsPtr(const Variant *v);
int Variant_IsConstPtr(const Variant *v);
int Variant_IsObjCId(const Variant *v);
int Variant_IsString(const Variant *v);
int Variant_IsArray(const Variant *v);

int Variant_IsAnyPrimitive(const Variant *v);
int Variant_IsAnyPointer(const Variant *v);
int Variant_IsArrayOrAnyPointer(const Variant *v);

Variant Variant_Null(void);
Variant Variant_True(void);
Variant Variant_False(void);
Variant Variant_Integer(vint64_t value);
Variant Variant_Boolean(int value);
Variant Variant_Real(double value);
Variant Variant_Ptr(void *value);
Variant Variant_ConstPtr(const void *value);
Variant Variant_ObjCId(void *value);
Variant Variant_String(const char *value);
Variant Variant_Array(Variant *array, int count);
    
const char *Variant_TypeString(VariantType type);
const char *Variant_StringDescription(const Variant *v);
    
#ifdef __cplusplus
}
#endif

#endif // _VARIANT_C_H