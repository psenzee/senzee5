//
//  SimpleCall.m
//

#import <UIKit/UIKit.h>
#import "SimpleCall.h"

@implementation SimpleCall

@synthesize selector;
@synthesize target;

- (id)initWithSelector:(SEL)sel forTarget:(id)targ
{
    self = [super init];
    if (self)
    {
        self.selector = sel;
        self.target = targ;
    }
    return self;
}

- (void)setSelector:(SEL)sel forTarget:(id)targ
{
    self.selector = sel;
    self.target = targ;
}

- (BOOL)isEqual:(id)other
{
    if (self == other)
        return YES;
    if (![other isKindOfClass:[SimpleCall class]])
        return NO;
    SimpleCall *oc = (SimpleCall *)other;
    return (oc.selector == self.selector && oc.target == self.target);
}

- (SimpleCall *)findEqualIn:(NSArray *)array
{
    if (array)
    {
        for (SimpleCall *c in array)
            if ([self isEqual:c])
                return c;
    }
    return nil;
}

- (BOOL)isCallable
{
    return self.selector && self.target && [self.target respondsToSelector:self.selector];
}

- (BOOL)call
{
    if ([self isCallable])
    {
        [self.target performSelector:self.selector];
        return YES;
    }
    return NO;
}

- (id)callAndReturnValue
{
    if ([self isCallable])
    {
        return [self.target performSelector:self.selector];
    }
    return nil;
}

- (BOOL)callWithObject:(id)object
{
    if ([self isCallable])
    {
        [self.target performSelector:self.selector withObject:object];
        return YES;
    }
    return NO;
}

- (id)callAndReturnValueWithObject:(id)object
{
    if ([self isCallable])
    {
        return [self.target performSelector:self.selector withObject:object];
    }
    return nil;
}

- (BOOL)callWithObject:(id)u withObject:(id)v
{
    if ([self isCallable])
    {
        [self.target performSelector:self.selector withObject:u withObject:v];
        return YES;
    }
    return NO;
}

- (BOOL)mainThreadCallWithObject:(id)object
{
    if ([self isCallable])
    {
        [self.target performSelectorOnMainThread:self.selector withObject:object waitUntilDone:NO];
        return YES;
    }
    return NO;
}

- (BOOL)mainThreadCall
{
    if ([self isCallable])
    {
        [self.target performSelectorOnMainThread:self.selector withObject:nil waitUntilDone:NO];
        return YES;
    }
    return NO;
}

- (BOOL)addToUIButton:(UIButton *)button forControlEvents:(int)events
{
    if (button && self.selector && self.target)
    {
        [button addTarget:self.target action:self.selector forControlEvents:events];
        return YES;
    }
    return NO;
}

- (BOOL)removeFromUIButton:(UIButton *)button forControlEvents:(int)events
{
    if (button && self.selector && self.target)
    {
        [button removeTarget:self.target action:self.selector forControlEvents:events];
        return YES;
    }
    return NO;
}

- (BOOL)addToUIButton:(UIButton *)button
{
    return [self addToUIButton:button forControlEvents:UIControlEventTouchUpInside];
}

- (BOOL)removeFromUIButton:(UIButton *)button
{
    return [self removeFromUIButton:button forControlEvents:UIControlEventTouchUpInside];
}

+ (void)doNothing
{
}

////////

+ (BOOL)call:(SimpleCall *)call
{
    return !call ? NO : [call call];
}

+ (id)callAndReturnValue:(SimpleCall *)call
{
    return !call ? nil : [call callAndReturnValue];
}

+ (BOOL)call:(SimpleCall *)call withObject:(id)object
{
    return !call ? NO : [call callWithObject:object];
}

+ (id)callAndReturnValue:(SimpleCall *)call withObject:(id)object
{
    return !call ? NO : [call callAndReturnValueWithObject:object];
}

+ (BOOL)call:(SimpleCall *)call withObject:(id)u withObject:(id)v
{
    return !call ? NO : [call callWithObject:u withObject:v];
}

+ (BOOL)mainThreadCall:(SimpleCall *)call
{
    return !call ? NO : [call mainThreadCall];
}

+ (BOOL)mainThreadCall:(SimpleCall *)call withObject:(id)object
{
    return !call ? NO : [call mainThreadCallWithObject:object];
}

+ (BOOL)isCallable:(SimpleCall *)call
{
    return !call ? NO : [call isCallable];
}

+ (SimpleCall *)callWithSelector:(SEL)sel forTarget:(id)targ
{
    return [[SimpleCall alloc] initWithSelector:sel forTarget:targ];
}

+ (SimpleCall *)nothing
{
    return [[SimpleCall alloc] initWithSelector:@selector(doNothing) forTarget:[SimpleCall class]];
}

@end

@interface Closure ()

@property (nonatomic, retain) SimpleCall *call;
@property (nonatomic, retain) id          objectu;
@property (nonatomic, retain) id          objectv;

- (id)initWithCall:(SimpleCall *)call withObject:(id)object;
- (id)initWithCall:(SimpleCall *)call withObject:(id)objectu withObject:(id)objectv;
- (BOOL)callWithObjects;

@end

@implementation Closure

- (id)initWithCall:(SimpleCall *)call_ withObject:(id)objectu withObject:(id)objectv
{
    self = [super init];
    if (self)
    {
        self.call = call_;
        self.objectu = objectu;
        self.objectv = objectv;
    }
    return self;
}

- (id)initWithCall:(SimpleCall *)call_ withObject:(id)object
{
    return [self initWithCall:call_ withObject:object withObject:nil];
}

- (BOOL)callWithObjects
{
    return [SimpleCall call:self.call withObject:self.objectu withObject:self.objectv];
}

+ (SimpleCall *)close:(SimpleCall *)call withObject:(id)object
{
    return [Closure close:call withObject:object withObject:nil];
}

+ (SimpleCall *)close:(SimpleCall *)call withObject:(id)objectu withObject:(id)objectv
{
    Closure *c = [[Closure alloc] initWithCall:call withObject:objectu withObject:objectv];
    SimpleCall *sc = [SimpleCall callWithSelector:@selector(callWithObjects) forTarget:c];
    return sc;
}

+ (SimpleCall *)close:(SEL)selector forTarget:(id)target withObject:(id)object
{
    return [self close:selector forTarget:target withObject:object withObject:nil];
}

+ (SimpleCall *)close:(SEL)selector forTarget:(id)target withObject:(id)objectu withObject:objectv
{
    return [self close:[SimpleCall callWithSelector:selector forTarget:target] withObject:objectu withObject:objectv];
}

@end
