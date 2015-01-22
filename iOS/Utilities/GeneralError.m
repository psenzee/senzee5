//
//  GeneralError.m
//

#import "GeneralError.h"

@implementation GeneralError

- (id)initWithIdentifier:(NSString *)identifier code:(int)code message:(NSString *)message
{
    self = [super init];
    if (self)
    {
        self.type       = ErrorType_Error;
        self.identifier = identifier;
        self.code       = code;
        self.message    = message;
    }
    return self;
}

@end
