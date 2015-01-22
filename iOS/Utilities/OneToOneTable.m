//
//  OneToOneTable.m
//

#import "OneToOneTable.h"

@implementation OneToOneTable

@synthesize keyToValue;
@synthesize valueToKey;

- (id)initWithObjects:(NSArray *)values forKeys:(NSArray *)keys
{
    self = [super init];
    if (self)
    {
        self.keyToValue = [[NSMutableDictionary alloc] initWithObjects:values forKeys:keys];
        self.valueToKey = [[NSMutableDictionary alloc] initWithObjects:keys   forKeys:values];
    }
    return self;
}

- (id)initWithKeysValues:(id)firstKey, ...
{
    self = [super init];
    if (!self)
        return self;
    
    id key = nil;
    int i = 0;

    self.keyToValue = [[NSMutableDictionary alloc] init];
    self.valueToKey = [[NSMutableDictionary alloc] init];
    
    va_list args;
    va_start(args, firstKey);
    for (NSString *arg = firstKey; arg != nil; arg = va_arg(args, id))
    {
        if ((i % 2) == 0)
            key = arg;
        else
        {
            [self.keyToValue setObject:arg forKey:key];
            [self.valueToKey setObject:key forKey:arg];
        }
        i++;
    }
    va_end(args);
    
    return self;
}

- (id)objectForKey:(id)key
{
    return !self.keyToValue ? nil : [self.keyToValue objectForKey:key];
}

- (id)keyForObject:(id)obj
{
    return !self.valueToKey ? nil : [self.valueToKey objectForKey:obj];
}

@end