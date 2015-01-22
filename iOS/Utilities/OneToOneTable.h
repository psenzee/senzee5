//
//  OneToOneTable.h
//
//  A two-way map, that allows keys and values to map to each other.  Especially useful for
//  drop-down menus/pickers, for example.

#import <Foundation/Foundation.h>

@interface OneToOneTable : NSObject

@property (nonatomic, retain) NSMutableDictionary *keyToValue;
@property (nonatomic, retain) NSMutableDictionary *valueToKey;

- (id)initWithObjects:(NSArray *)values forKeys:(NSArray *)keys;
- (id)initWithKeysValues:(id)firstKey, ... NS_REQUIRES_NIL_TERMINATION; // key1, value1, key2, value2, .., keyn, valuen, nil
- (id)objectForKey:(id)key;
- (id)keyForObject:(id)obj;

+ (OneToOneTable *)tableWithObjects:(NSArray *)values forKeys:(NSArray *)keys;

@end

