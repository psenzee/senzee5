//
//  DataUtil.m
//

#import "DataUtil.h"
#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h> // Need to import for CC_MD5 access

BOOL StrIsIn(NSString *string, NSArray *array)
{
    return string && array && [array containsObject:string];
}

BOOL StrEq(NSString *str1, NSString *str2)
{
    return (str1 == str2) || (str1 && str2 && [str1 isEqualToString:str2]);
}

BOOL Dict_HasKey(NSDictionary *dictionary, id key)
{
    return dictionary && [[dictionary allKeys] containsObject:key];
}

id Dict_ObjectForKeyWithDefault(NSDictionary *dictionary,
                                      id key, id defaultObject)
{
    return Dict_HasKey(dictionary, key) ? [dictionary objectForKey:key] : defaultObject;
}

id Dict_ObjectForKey(NSDictionary *dictionary, id key)
{
    return Dict_ObjectForKeyWithDefault(dictionary, key, nil);
}

id Dict_ObjectForKeyOfOptionalType(NSDictionary *dictionary, id key, Class *type)
{
    return Dict_HasKey(dictionary, key) ? SafeTypeOptional([dictionary objectForKey:key], type) : nil;
}

id Dict_ObjectForKeyOfType(NSDictionary *dictionary, id key, Class type)
{
    return Dict_ObjectForKeyOfOptionalType(dictionary, key, &type);
}

BOOL Array_AddObject(NSMutableArray *array, id object)
{
    if (object)
    {
        [array addObject:object];
        return YES;
    }
    return NO;
}

id SafeType(id value, Class type)
{
    return (!value || ![value isKindOfClass:type]) ? nil : value;
}

id SafeTypeOptional(id value, Class *type)
{
    return !type ? value : SafeType(value, *type);
}

NSString *SafeStr(id value)
{
    return (NSString *)SafeType(value, [NSString class]);
}

NSString *SafeStrCoerced(id value)
{
    NSString *str = SafeStr(value);
    return str ? str : [NSString stringWithFormat:@"%@", value];
}

NSNumber *SafeNum(id value)
{
    return (NSNumber *)SafeType(value, [NSNumber class]);
}

NSArray *SafeArray(id value)
{
    return (NSArray *)SafeType(value, [NSArray class]);
}

NSMutableArray *SafeMutableArray(id value)
{
    return (NSMutableArray *)SafeType(value, [NSMutableArray class]);
}

NSDictionary *SafeDict(id value)
{
    return (NSDictionary *)SafeType(value, [NSDictionary class]);
}

NSMutableDictionary *SafeMutableDict(id value)
{
    return (NSMutableDictionary *)SafeType(value, [NSMutableDictionary class]);
}

NSNumber *SafeNumCoerced(id value)
{
    NSString *str = SafeStr(value);
    if (str)
    {
        NSNumberFormatter *f = [[NSNumberFormatter alloc] init];
        [f setNumberStyle:NSNumberFormatterDecimalStyle];
        NSNumber *number = [f numberFromString:str];
        [f release];
        return number;
    }
    return SafeNum(value);
}

id FirstValueForKeyOfType(id obj, NSString *key, Class *type)
{
    NSArray *array = nil;
    id       value = nil;
    if ([obj isKindOfClass:[NSArray class]])
        array = (NSArray *)obj;
    else if ([obj isKindOfClass:[NSDictionary class]])
        if ((value = Dict_ObjectForKeyOfOptionalType((NSDictionary *)obj, key, type)) == nil)
            array = [(NSDictionary *)obj allValues];
    for (id object in array)
        if ((value = FirstValueForKeyOfType(object, key, type)) != nil)
            break;
    return value;
}

id FirstValueForKey(id obj, NSString *key)
{
    return FirstValueForKeyOfType(obj, key, nil);
}

// depth-first, perhaps should be breadth-first
void ListValuesForKeyOfType(id obj, NSString *key, NSMutableArray *into, Class *type)
{
    NSArray *array = nil;
    if ([obj isKindOfClass:[NSArray class]])
        array = (NSArray *)obj;
    else if ([obj isKindOfClass:[NSDictionary class]])
    {
        Array_AddObject(into, Dict_ObjectForKeyOfOptionalType((NSDictionary *)obj, key, type));
        array = [(NSDictionary *)obj allValues];
    }
    for (id object in array)
        ListValuesForKeyOfType(object, key, into, type);
}

void ListValuesForKey(id obj, NSString *key, NSMutableArray *into)
{
    ListValuesForKeyOfType(obj, key, into, nil);
}

NSData *AllocateDataWithContentsOfUrl(NSURL *url)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSData *data = nil, *original = [NSData dataWithContentsOfURL:url];
    if (original)
        data = [[NSData alloc] initWithData:original];
    [pool release];
    return data;
}

NSArray *AllocateArrayWithComponentsSeparatedByString(NSString *s, NSString *separator)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSArray *data = nil, *original = [s componentsSeparatedByString:separator];
    data = [[NSArray alloc] initWithArray:original];
    [pool release];
    return data;
}

NSData *AllocateDataWithUTF8String(NSString *s)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSData *data = nil, *original = [s dataUsingEncoding:NSUTF8StringEncoding];
    if (original)
        data = [[NSData alloc] initWithData:original];
    [pool release];
    return data;
}

NSMutableArray *DeepCopyStringArray(NSArray *array)
{
    NSMutableArray *result = [[NSMutableArray alloc] initWithCapacity:array.count];
    for (NSString *s in array)
    {
        NSString *scopy = [[NSString alloc] initWithString:s];
        [result addObject:scopy];
    }
    return result;
}

NSString *DataToBase64String(NSData *data)
{
#if __IPHONE_7_0
    return [[NSString alloc] initWithString:[data base64EncodedStringWithOptions:0]]; // iOS7.0+
#else
    return [[NSString alloc] initWithString:[data base64Encoding]];
#endif
}

NSData *Base64StringToData(NSString *base64)
{
#if __IPHONE_7_0
    return [[NSData alloc] initWithBase64EncodedString:base64 options:0]; // iOS7.0+
#else
    return [[NSData alloc] initWithBase64Encoding:base64];
#endif
}

NSString *ToHex(NSData *token)
{
    int size = [token length];
    const char *data = (const char *)[token bytes];
    char *hex = (char *)malloc(size * 2 + 1);
    char *phex = hex;
    *phex = '\0';
    char hexchar[] = "0123456789abcdef";    
    for (int i = 0; i < size; i++)
    {
        unsigned char value = (unsigned char)data[i];
        *phex = hexchar[(value >> 4) & 0xf]; phex++;
        *phex = hexchar[value & 0xf]; phex++;
        *phex = '\0';
    }
    NSString *str = [NSString stringWithUTF8String:(const char *)hex];
    free(hex);
    return str;
}

NSComparisonResult ReverseComparison(NSComparisonResult compare)
{
    if (compare == NSOrderedAscending)  return NSOrderedDescending;
    if (compare == NSOrderedDescending) return NSOrderedAscending;
    return NSOrderedSame;
}

NSString *GetMD5HashForString(NSString *input)
{
    unsigned char result[CC_MD5_DIGEST_LENGTH];
    // Convert NSString into C-string and generate MD5 Hash
    CC_MD5([input UTF8String], [input length], result);
    // Convert C-string (the hash) into NSString
    NSMutableString *hash = [NSMutableString string];
    for (int i = 0; i < CC_MD5_DIGEST_LENGTH; i++)
        [hash appendFormat:@"%02x", result[i]];
    return [NSString stringWithString:hash];
}

NSString *GetUserDefaultValue(NSString *key)
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    return [defaults objectForKey:key];
}

id SetUserDefaultValue(NSString *key, id object)
{
    if (!key) return nil;
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    if (object)
    {
        [defaults setObject:object forKey:key];
        [defaults synchronize];
    }
    else
    {
        if (GetUserDefaultValue(key) != nil)
            [defaults removeObjectForKey:key];
    }
    return object;
}

BOOL ObjectEqual(id a, id b)
{
    return (a == b || (a != nil && b != nil && [a isEqual:b]));
}

id SetUserDefaultValueIfDiffers(NSString *key, id object)
{
    if (!ObjectEqual(GetUserDefaultValue(key), object))
        SetUserDefaultValue(key, object);
    return object;
}

id LoadUserDefaultValueIfNil(NSString *key, id currentValue)
{
    if (!currentValue)
        return GetUserDefaultValue(key);
    SetUserDefaultValueIfDiffers(key, currentValue);
    return currentValue;
}
