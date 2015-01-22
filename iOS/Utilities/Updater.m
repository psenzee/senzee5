//
//  Updater.m
//

#import "Updater.h"

@interface Updater ()

@property          BOOL           done;
@property          BOOL           completed;
@property          BOOL           executing;
@property          NSTimeInterval sleep;
@property          NSTimeInterval interval;
@property          BOOL           executeOnMainThread;
@property (retain) NSThread      *thread;
@property (retain) id             target;
@property          SEL            selector;

- (id)initWithObject:(id)object selector:(SEL)selector interval:(NSTimeInterval)interval;
- (void)run;

@end

@implementation Updater

- (id)initWithObject:(id)object selector:(SEL)selector interval:(NSTimeInterval)inter
{
	self = [super init];
    if (self)
    {
        self.target    = object;
        self.selector  = selector;
        self.interval  = inter;
        self.sleep     = 0.0;
        self.done      = NO;
        self.completed = NO;
        self.executing = NO;
        self.executeOnMainThread = NO;
    }
	return self;
}

- (void)sleep:(NSTimeInterval)s
{
    self.sleep = s;
}

- (void)doDelay
{
    if (self.sleep)
    {
        [NSThread sleepForTimeInterval:self.sleep];
        self.sleep = 0.0;
    }
}

- (void)run
{
    if (!self.thread)
        self.thread = [NSThread currentThread];
    if (self.executing)
        return;
    self.executing = YES;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    self.completed = NO;
    while (!self.done)
    {
        [self doDelay];
        [NSThread sleepForTimeInterval:self.interval];
        [self doDelay];        
        if (self.target && self.selector)
        {
            if (self.executeOnMainThread && ![NSThread isMainThread])
                [self.target performSelectorOnMainThread:self.selector 
                                              withObject:nil waitUntilDone:YES];
            else
                [self.target performSelector:self.selector withObject:nil];
        }
    }
    self.completed = YES;
    self.executing = NO;
    [pool drain];
}

- (void)invalidate
{
    self.done = YES;
}

- (void)stop // this blocks, cannot be called on same thread as run
{
    [self retain];
    if (self.completed || !self.executing)
    {
        [self release];
        return;
    }
    [self invalidate];
    // spin on this and wait until complete
    while (!self.completed)
        [NSThread sleepForTimeInterval:0.01];
    [self release];
}

+ (Updater *)updaterWithObject:(id)object selector:(SEL)selector interval:(NSTimeInterval)interval onMainThread:(BOOL)onMainThread
{
    Updater *u = [[Updater alloc] initWithObject:object selector:selector interval:interval];
    u.executeOnMainThread = onMainThread;
    [NSThread detachNewThreadSelector:@selector(run) toTarget:u withObject:nil];
    return u;
}

+ (Updater *)updaterWithObject:(id)object selector:(SEL)selector interval:(NSTimeInterval)interval
{
    return [self updaterWithObject:object selector:selector interval:interval onMainThread:NO];
}

@end