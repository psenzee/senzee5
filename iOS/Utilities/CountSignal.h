//
//  CountSignal.h
//

// Invokes a selector when its selector has been invoked a given number of times

#import <Foundation/Foundation.h>

@interface CountSignal : NSObject

@property                     int count;
@property                     int threshold;
@property (nonatomic)         SEL selector;
@property (nonatomic, retain) id  target;

- (id)initWithThreshold:(int)threshold forSelector:(SEL)selector forTarget:(id)target;
- (void)tally;

@end

