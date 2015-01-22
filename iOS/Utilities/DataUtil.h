//
//  DataUtil.h
//
//  Various convenience utilities for type-safe manipulation of values

#import <Foundation/Foundation.h>

BOOL                 StrIsIn(NSString *string, NSArray *array);
BOOL                 StrEq(NSString *str1, NSString *str2);

BOOL                 Dict_HasKey(NSDictionary *dictionary, id key);
id                   Dict_ObjectForKey(NSDictionary *dictionary, id key);
id                   Dict_ObjectForKeyWithDefault(NSDictionary *dictionary, id key, id defaultObject);
id                   Dict_ObjectForKeyOfOptionalType(NSDictionary *dictionary, id key, Class *type);
id                   Dict_ObjectForKeyOfType(NSDictionary *dictionary, id key, Class type);

BOOL                 Array_AddObject(NSMutableArray *array, id object);

id                   FirstValueForKey(id obj, NSString *key);
id                   FirstValueForKeyOfType(id obj, NSString *key, Class *type);
void                 ListValuesForKey(id obj, NSString *key, NSMutableArray *into);
void                 ListValuesForKeyOfType(id obj, NSString *key, NSMutableArray *into, Class *type);

NSData              *AllocateDataWithContentsOfUrl(NSURL *url);
NSArray             *AllocateArrayWithComponentsSeparatedByString(NSString *s, NSString *separator);
NSData              *AllocateDataWithUTF8String(NSString *s);

NSMutableArray      *DeepCopyStringArray(NSArray *array);

NSString            *DataToBase64String(NSData *data);
NSData              *Base64StringToData(NSString *base64);

NSString            *ToHex(NSData *token);

NSComparisonResult   ReverseComparison(NSComparisonResult compare);

NSString            *GetMD5HashForString(NSString *input);

NSString            *GetUserDefaultValue(NSString *key);
id                   SetUserDefaultValue(NSString *key, id object);
BOOL                 ObjectEqual(id a, id b);
id                   SetUserDefaultValueIfDiffers(NSString *key, id object);
id                   LoadUserDefaultValueIfNil(NSString *key, id currentValue);

id                   SafeType(id value, Class type);
id                   SafeTypeOptional(id value, Class *type);
NSString            *SafeStr(id value);
NSString            *SafeStrCoerced(id value);
NSNumber            *SafeNum(id value);
NSNumber            *SafeNumCoerced(id value);
NSArray             *SafeArray(id value);
NSDictionary        *SafeDict(id value);
NSMutableArray      *SafeMutableArray(id value);
NSMutableDictionary *SafeMutableDict(id value);