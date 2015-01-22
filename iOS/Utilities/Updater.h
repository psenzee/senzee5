//
//  Updater.h
//
//  Updater - create an updater instance to call a specified selector on an instance
//  over and over at the specified interval until stopped.  
//  Optionally calls it on the main thread.

#import <Foundation/Foundation.h>

@interface Updater : NSObject

- (void)stop;
- (void)sleep:(NSTimeInterval)sleep;
- (void)invalidate;

+ (Updater *)updaterWithObject:(id)object selector:(SEL)selector interval:(NSTimeInterval)interval;
+ (Updater *)updaterWithObject:(id)object selector:(SEL)selector interval:(NSTimeInterval)interval
                          onMainThread:(BOOL)onMainThread;

@end

