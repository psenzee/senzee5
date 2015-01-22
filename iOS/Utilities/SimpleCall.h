//
//  SimpleCall.h
//
//  SimpleCall encapsulates the invocation of a no-argument selector on a target.  
//  Closure encapsulates the invocation of a one- or two-argument selector on a target.
//  These are not as often necessary these days with so much support for blocks.

#import <Foundation/Foundation.h>

@class UIButton;

@interface SimpleCall : NSObject

@property (nonatomic)         SEL selector;
@property (nonatomic, retain) id  target;

- (id)initWithSelector:(SEL)selector forTarget:(id)target;
- (void)setSelector:(SEL)selector forTarget:(id)target;
- (BOOL)call;
- (id)callAndReturnValue;
- (BOOL)callWithObject:(id)object;
- (id)callAndReturnValueWithObject:(id)object;
- (BOOL)callWithObject:(id)objecu withObject:(id)objecv;
- (BOOL)mainThreadCall;
- (BOOL)mainThreadCallWithObject:(id)object;
- (BOOL)isCallable;

- (BOOL)isEqual:(id)object;

- (BOOL)addToUIButton:(UIButton *)button;
- (BOOL)addToUIButton:(UIButton *)button forControlEvents:(int)events;
- (BOOL)removeFromUIButton:(UIButton *)button;
- (BOOL)removeFromUIButton:(UIButton *)button forControlEvents:(int)events;

- (SimpleCall *)findEqualIn:(NSArray *)array;

// class methods check for nil
+ (BOOL)call:(SimpleCall *)call;
+ (id)callAndReturnValue:(SimpleCall *)call;
+ (BOOL)call:(SimpleCall *)call withObject:(id)object;
+ (id)callAndReturnValue:(SimpleCall *)call withObject:(id)object;
+ (BOOL)call:(SimpleCall *)call withObject:(id)objecu withObject:(id)objecv;
+ (BOOL)mainThreadCall:(SimpleCall *)call;
+ (BOOL)mainThreadCall:(SimpleCall *)call withObject:(id)object;
+ (BOOL)isCallable:(SimpleCall *)call;

+ (SimpleCall *)callWithSelector:(SEL)selector forTarget:(id)target;
+ (SimpleCall *)nothing;

@end

@interface Closure : NSObject

+ (SimpleCall *)close:(SimpleCall *)call withObject:(id)object;
+ (SimpleCall *)close:(SimpleCall *)call withObject:(id)objectu withObject:(id)objectv;

+ (SimpleCall *)close:(SEL)selector forTarget:(id)target withObject:(id)object;
+ (SimpleCall *)close:(SEL)selector forTarget:(id)target withObject:(id)objectu withObject:objectv;

@end


