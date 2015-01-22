//
//  EventWatcher.h
//
//  An (potentially singleton) class for dispatching events to communicate amoung
//  elements at different architectural strata

#import <Foundation/Foundation.h>

@class SimpleCall;

@interface EventWatcher : NSObject

@property (nonatomic, retain) NSMutableDictionary *keys;
@property (nonatomic, retain) NSMutableDictionary *updaters;

- (id)init;

- (void)addEventForKeys:(NSString *)keys withCall:(SimpleCall *)call;

- (BOOL)removeEventsWithCall:(SimpleCall *)call;
- (BOOL)removeEventsWithTarget:(id)target;

- (BOOL)removeEventsForKeys:(NSString *)keys;
- (BOOL)removeEventsForKeys:(NSString *)keys withCall:(SimpleCall *)call;
- (BOOL)removeEventsForKeys:(NSString *)keys withTarget:(id)target;

- (void)fireEventsForKeys:(NSString *)keys;
- (void)fireEventsOnMainThreadForKeys:(NSString *)keys;

- (BOOL)startRecurringEventWithName:(NSString *)name forKeys:(NSString *)keyset
                     atTimeInterval:(NSTimeInterval)interval mainThread:(BOOL)mainThread;
- (BOOL)stopRecurringEventWithName:(NSString *)name;

+ (EventWatcher *)shared;

@end

