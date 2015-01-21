//
//  Json.h
//
//

#import "Json.h"
#import <Foundation/Foundation.h>
#import "strs_c.h"

BOOL ParseJSONString(const char **json, NSObject **idout, BOOL allowNull);
BOOL ParseJSONPrimitive(const char **json, NSObject **idout);
BOOL ParseJSONValue(const char **json, NSObject **idout);
BOOL ParseJSONPrimitive(const char **json, NSObject **idout);
BOOL ParseJSONDictionary(const char **json, NSObject **idout);
BOOL ParseJSONArray(const char **json, NSObject **idout);

BOOL ParseJSONString(const char **json, NSObject **idout, BOOL allowNull)
{
    if (!*json || !**json)
        return NO;
    const char *origin = *json;
    char *qvalue = strs_tmp_buffer(strlen(origin));
    *idout = nil;
    BOOL result = YES;
    if (strs_read_quoted(json, '"', qvalue, true))
        *idout = [NSString stringWithUTF8String:qvalue];
    else if (allowNull && strs_is(json, "null", false))
        *idout = nil;
    else if (*idout == nil)
    {
        *json = origin;
        result = NO;
    }
    strs_free_tmp_buffer(qvalue);
    return result;
}

BOOL ParseJSONPrimitive(const char **json, NSObject **idout)
{
    const char *origin = *json;
    double dvalue = 0.f;
    *idout = nil;
    if (strs_read_double(json, &dvalue))
        *idout = [NSNumber numberWithDouble:dvalue];
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

BOOL ParseJSONDictionary(const char **json, NSObject **idout)
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
        if (!ParseJSONValue(json, &key))
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
        if (!ParseJSONValue(json, &value))
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

BOOL ParseJSONArray(const char **json, NSObject **idout)
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
        if (!ParseJSONValue(json, &value))
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

BOOL ParseJSONValue(const char **json, NSObject **idout)
{
    return ParseJSONString(json, idout, _true) ||
           ParseJSONPrimitive(json, idout) ||
           ParseJSONArray(json, idout) ||
           ParseJSONDictionary(json, idout);
}

NSObject *ParseJSON(NSString *json)
{
    const char *utf8 = [json UTF8String];
    NSObject *value = nil;
    ParseJSONValue(&utf8, &value);
    return value;
}

id JSONValue(id value)
{
    return value ? value : @"/nil/";
}

void SerializeJSON(NSMutableString *str, id value)
{
    if (value == nil || [value isEqual:@"/nil/"])
    {
        [str appendString:@"null"];
    }
    else if ([value isKindOfClass:[NSArray class]])
    {
        [str appendString:@"["];
        BOOL first = YES;
        for (id obj in (NSArray *)value)
        {
            if (!first)
                [str appendString:@","];
            first = NO;
            SerializeJSON(str, obj);
        }
        [str appendString:@"]"];
    }
    else if ([value isKindOfClass:[NSDictionary class]])
    {
        [str appendString:@"{"];
        BOOL first = YES;
        NSDictionary *dict = (NSDictionary *)value;
        for (NSString *key in [dict allKeys])
        {
            if (!first)
                [str appendString:@","];
            first = NO;
            id kvalue = [dict objectForKey:key];
            [str appendFormat:@"\"%@\":", key];
            SerializeJSON(str, kvalue);
        }
        [str appendString:@"}"];
    }
    else if ([value isKindOfClass:[NSString class]])
    {
        [str appendFormat:@"\"%@\"", (NSString *)value];
    }
    else if ([value isKindOfClass:[NSNumber class]])
    {
        NSNumber *n = (NSNumber *)value;
        if (n.objCType[0] == 'c') // if it comes from a char-size, assume it's a bool
            [str appendFormat:@"%@", n.boolValue ? @"true" : @"false"];
        else
            [str appendFormat:@"%@", (NSNumber *)value];
    }
    else
    {
        [str appendFormat:@"\"%@\"", value];
    }
}
