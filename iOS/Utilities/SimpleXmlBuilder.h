//
//  SimpleXmlBuilder.h
//
//  Simple stack-based class for building an XML string by adding tags and text.

#import <Foundation/Foundation.h>

@interface SimpleXmlBuilder : NSObject

@property (nonatomic, retain) NSMutableString *xml;

- (id)init;
- (void)beginTag:(NSString *)name;
- (void)addText:(NSString *)text;
- (void)endTag;
- (NSString *)complete;

@end

