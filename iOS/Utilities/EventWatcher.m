//
//  EventWatcher.m
//

#import <Foundation/Foundation.h>
#import "EventWatcher.h"
#import "EventList.h"
#import "SimpleCall.h"
#import "SysUtil.h"
#import "Updater.h"
#import "types_c.h"

static EventWatcher *SHARED_INSTANCE;

@implementation EventWatcher

+ (EventWatcher *)shared
{
    if (!SHARED_INSTANCE)
        SHARED_INSTANCE = [[EventWatcher alloc] init];
    return SHARED_INSTANCE;
}

- (id)init
{
    self = [super init];
    if (self)
    {
        self.keys     = [[NSMutableDictionary alloc] init];
        self.updaters = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (EventList *)eventListForKey:(NSString *)key
{
    return (EventList *)[self.keys objectForKey:key];
}

- (EventList *)createOrReturnEventListForKey:(NSString *)key
{
    EventList *el = [self eventListForKey:key];
    if (!el)
    {
        el = [[EventList alloc] init];
        [self.keys setObject:el forKey:key];
    }
    return el;
}

- (void)_addEventForKey:(NSString *)key withCall:(SimpleCall *)call
{
    EventList *el = [self createOrReturnEventListForKey:key];
    [el add:call];
}

- (void)addEventForKeys:(NSString *)keyset withCall:(SimpleCall *)call
{
    if (keyset)
    {
        NSRange r = [keyset rangeOfString:@","];
        if (r.location == NSNotFound)
            [self _addEventForKey:keyset withCall:call];
        else
        {
            NSArray *list = [keyset componentsSeparatedByString:@","];
            for (NSString *key in list)
                [self _addEventForKey:key withCall:call];
        }
    }
}

- (BOOL)_removeEventForKey:(NSString *)key withCall:(SimpleCall *)call
{
    EventList *el = [self createOrReturnEventListForKey:key];
    return [el remove:call];
}

- (BOOL)_removeEventsForKey:(NSString *)key withTarget:(id)target
{
    EventList *el = [self createOrReturnEventListForKey:key];
    return [el removeForTarget:target];
}

- (BOOL)_removeEventsForKey:(NSString *)key
{
    EventList *el = [self createOrReturnEventListForKey:key];
    return [el removeAll];
}

- (BOOL)removeEventsWithCall:(SimpleCall *)call
{
    BOOL removed = NO;
    for (NSString *key in [self.keys keyEnumerator])
        removed |= [self _removeEventForKey:key withCall:call];
    return removed;
}

- (BOOL)removeEventsWithTarget:(id)target
{
    BOOL removed = NO;
    for (NSString *key in [self.keys keyEnumerator])
        removed |= [self _removeEventsForKey:key withTarget:target];
    return removed;
}

- (BOOL)removeEventsForKeys:(NSString *)keyset withCall:(SimpleCall *)call
{
    if (keyset)
    {
        NSRange r = [keyset rangeOfString:@","];
        if (r.location == NSNotFound)
            return [self _removeEventForKey:keyset withCall:call];
        else
        {
            BOOL removed = NO;
            NSArray *list = [keyset componentsSeparatedByString:@","];
            for (NSString *key in list)
                removed |= [self _removeEventForKey:key withCall:call];
            return removed;
        }
    }
    return NO;
}

- (BOOL)removeEventsForKeys:(NSString *)keyset withTarget:(id)target
{
    if (keyset)
    {
        NSRange r = [keyset rangeOfString:@","];
        if (r.location == NSNotFound)
            return [self _removeEventsForKey:keyset withTarget:target];
        else
        {
            BOOL removed = NO;
            NSArray *list = [keyset componentsSeparatedByString:@","];
            for (NSString *key in list)
                removed |= [self _removeEventsForKey:key withTarget:target];
            return removed;
        }
    }
    return NO;
}

- (BOOL)removeEventsForKeys:(NSString *)keyset
{
    if (keyset)
    {
        NSRange r = [keyset rangeOfString:@","];
        if (r.location == NSNotFound)
            return [self _removeEventsForKey:keyset];
        else
        {
            BOOL removed = NO;
            NSArray *list = [keyset componentsSeparatedByString:@","];
            for (NSString *key in list)
                removed |= [self _removeEventsForKey:key];
            return removed;
        }
    }
    return NO;
}

- (void)_fireEventsForKey:(NSString *)key
{
    EventList *el = [self eventListForKey:key];
    if (el) [el fire];
}

- (void)fireEventsForKeys:(NSString *)keyset
{
    if (keyset)
    {
        NSRange r = [keyset rangeOfString:@","];
        if (r.location == NSNotFound)
            [self _fireEventsForKey:keyset];
        else
        {
            NSArray *list = [keyset componentsSeparatedByString:@","];
            for (NSString *key in list)
                [self _fireEventsForKey:key];
        }
    }
}

- (void)fireEventsOnMainThreadForKeys:(NSString *)key
{
    PerformOnMainThread(self, @selector(fireEventsForKeys:), key, NO);
}

- (Updater *)updaterWithName:(NSString *)name
{
    if (![[self.updaters allKeys] containsObject:name])
        return nil;
    return (Updater *)[self.updaters objectForKey:name];
}

- (BOOL)startRecurringEventWithName:(NSString *)name forKeys:(NSString *)keyset
                atTimeInterval:(NSTimeInterval)interval mainThread:(BOOL)mainThread
{
    if ([self updaterWithName:name])
    {
        // already exists
        return NO;
    }
    SimpleCall *firer = [Closure close:@selector(fireEventsForKeys:) forTarget:self withObject:keyset];
    Updater *us = [Updater updaterWithObject:firer.target selector:firer.selector
                                                    interval:interval onMainThread:mainThread];
    [self.updaters setObject:us forKey:name];
    return YES;
}

@end

void EventWatcher_fireEventsForKeys(const char *keys, _bool onMainThread)
{
    NSString *nskeys = [NSString stringWithFormat:@"%s", keys];
    if (onMainThread)
        [[EventWatcher shared] fireEventsOnMainThreadForKeys:nskeys];
    else
        [[EventWatcher shared] fireEventsForKeys:nskeys];
}
