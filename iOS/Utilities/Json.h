//
//  Json.h
//
//  Dead simple utilities for parsing JSON text.
// 

#import <Foundation/Foundation.h>

NSObject *ParseJSON(NSString *json);

id        JSONValue(id value);
void      SerializeJSON(NSMutableString *str, id value);
