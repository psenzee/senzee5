//
//  SimpleXmlBuilder.m
//

#import "SimpleXmlBuilder.h"

@interface SimpleXmlBuilder ()

@property (nonatomic, retain) NSMutableArray *stack;

@end

@implementation SimpleXmlBuilder

@synthesize xml;

- (id)init
{
    self = [super init];
    if (self)
    {
        self.xml = [[NSMutableString alloc] init];
        _stack = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)beginTag:(NSString *)name
{
    [self.stack addObject:name];
    [self.xml appendFormat:@"<%@>", name];
}

- (void)addText:(NSString *)text
{
    if (text)
        [self.xml appendString:text];
}

- (void)endTag
{
    if ([self.stack count])
    {
        NSString *top = [self.stack lastObject];
        [self.xml appendFormat:@"</%@>", top];
        [self.stack removeLastObject];
    }
}

- (NSString *)complete
{
    while ([self.stack count])
        [self endTag];
    return [[NSString alloc] initWithString:self.xml];
}

@end
