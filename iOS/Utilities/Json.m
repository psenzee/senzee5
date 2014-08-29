// Objective-C JSON Utilities, Paul D. Senzee (c) 2011-2014

#import "Json.h"
#import <Foundation/Foundation.h>
#import "strs_c.h"

BOOL DeserializeJsonString(const char **json, NSObject **idout, BOOL allowNull);
BOOL DeserializeJsonPrimitive(const char **json, NSObject **idout);
BOOL DeserializeJsonValue(const char **json, NSObject **idout);
BOOL DeserializeJsonPrimitive(const char **json, NSObject **idout);
BOOL DeserializeJsonDictionary(const char **json, NSObject **idout);
BOOL DeserializeJsonArray(const char **json, NSObject **idout);

BOOL DeserializeJsonString(const char **json, NSObject **idout, BOOL allowNull)
{
    const char *origin = *json;
    char *str = NULL;
    
    size_t strsize = strlen(origin);
    if (strsize == 0)
        return NO;
    str = (char *)malloc(strsize + 1);
    memset(str, 0, strsize + 1);
    
    *idout = nil;
    if (strs_read_quoted(json, '"', str, _true))
    {
        *idout = [NSString stringWithUTF8String:str];
        free(str);
    }
    else if (allowNull && strs_is(json, "null", _false))
    {
        *idout = nil;
        free(str);
        return YES;
    }
    if (*idout == nil)
    {
        *json = origin;
        free(str);
        return NO;
    }
    free(str);
    return YES;
}

BOOL DeserializeJsonPrimitive(const char **json, NSObject **idout)
{
    const char *origin = *json;
    float fvalue = 0.f;
    *idout = nil;
    if (strs_read_float(json, &fvalue))
        *idout = [NSNumber numberWithFloat:fvalue];
    else if (strs_is(json, "false", _false))
        *idout = [NSNumber numberWithInt:0];
    else if (strs_is(json, "true", _false))
        *idout = [NSNumber numberWithInt:1];
    else if (strs_is(json, "null", _false))
    {
        *idout = nil;
        return YES;
    }
    if (*idout == nil)
    {
        *json = origin;
        return NO;
    }
    return YES;
}

BOOL DeserializeJsonDictionary(const char **json, NSObject **idout)
{
    const char *origin = *json; 
    *idout = nil;
    if (!strs_is(json, "{", _false))
    {
        *json = origin;
        return NO;
    }
    NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
    while (**json)
    {
        
        NSObject *key = nil;
        if (!DeserializeJsonValue(json, &key))
        {
            if (strs_is(json, "}", _false))
            {
                *idout = dict;
                return YES;
            }
            break;
        }
        if (!strs_is(json, ":", _false))
            break;
        NSObject *value = nil;
        if (!DeserializeJsonValue(json, &value))
            break;
        if (value == nil)
            value = @"/nil/";
        if ([key conformsToProtocol:@protocol(NSCopying)])
            [dict setObject:value forKey:(id<NSCopying>)key];
        if (!strs_is(json, ",", _false))
        {
            if (!strs_is(json, "}", _false))
                break;
            *idout = dict;
            return YES;
        }
    }
    [dict release]; 
    *json = origin; 
    return NO;
}

BOOL DeserializeJsonArray(const char **json, NSObject **idout)
{   
    const char *origin = *json; 
    *idout = nil;
    if (!strs_is(json, "[", _false))
    {
        *json = origin;
        return NO;
    }
    NSMutableArray *array = [[NSMutableArray alloc] init];
    while (**json)
    {
        NSObject *value = nil;
        if (!DeserializeJsonValue(json, &value))
        {
            if (strs_is(json, "]", _false))
            {
                *idout = array;
                return YES;
            }
            break;
        }
        [array addObject:value];
        if (!strs_is(json, ",", _false))
        {
            if (!strs_is(json, "]", _false))
                break;
            *idout = array;
            return YES;
        }
    }
    [array release];    
    *json = origin; 
    return NO;
}

// Top down parse - attempt to parse JSON Values starting with primitives
// and graduating to composite values, such as arrays and dictionaries,
// which will recursively call this function for nested values
BOOL DeserializeJsonValue(const char **json, NSObject **idout)
{
    return DeserializeJsonString(json, idout, _true) ||
           DeserializeJsonPrimitive(json, idout) ||
           DeserializeJsonArray(json, idout) ||
           DeserializeJsonDictionary(json, idout); 
}

// Read JSON text and produce a deserialized version 
// composed of NSNumber, NSString, NSArray and NSDictionary objects
NSObject *DeserializeJson(NSString *json)
{
    const char *utf8 = [json UTF8String];
    NSObject *value = nil;
    DeserializeJsonValue(&utf8, &value);
    return value;
}
