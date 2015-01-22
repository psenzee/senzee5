//
//  CountSignal.m
//

#import "CountSignal.h"

@implementation CountSignal

@synthesize count;
@synthesize threshold;
@synthesize selector;
@synthesize target;

- (id)initWithThreshold:(int)thethreshold forSelector:(SEL)theselector forTarget:(id)thetarget
{
    self = [super init];
    if (self)
    {
        self.threshold = thethreshold;
        self.selector = theselector;
        self.target = thetarget;
        self.count = 0;
    }
    return self;
}

- (void)tally
{
    self.count++;
    if (self.count >= self.threshold)
        [self.target performSelector:self.selector];
}

@end
