//
//  SysUtil.h
//
//  Threading and various other system related convenience functions.

#import <Foundation/Foundation.h>

BOOL                 PerformIfResponds(id object, SEL selector);
void                 ReleaseOnMainThread(NSObject *object);
void                 MainThreadCheck(NSString *where);
void                 ReleaseAndRemoveAllObjects(NSMutableArray *array);
void                 ReleaseAndRemoveAllObjectsFromDictionary(NSMutableDictionary *dict);

void                 PerformBlockOnMainThread(void (^block)());
void                 PerformOnMainThread(id object, SEL selector, id withObject, BOOL wait);
void                 PerformOnMainThreadAfterDelay(id object, SEL selector, id withObject, NSTimeInterval delay);

void                 PrintRefCountForDictionary(NSMutableDictionary *dict);

NSString            *GenerateUUID(void);
NSString            *GetUserUniqueUDID(void);
NSString            *GetUserUniqueUUID(void);

void                 ReadInfoDictionary(NSMutableDictionary *properties);

void                 GoToUrlInBrowser(NSString *url);

void                 PrintFreeMemory(void);
double               GetFreeMemory(void);

void                 ListenForPropertyChange(NSString *property, NSObject *object, id target, SEL changeSelector);
void                 ListenForBooleanPropertyChange(NSString *property, NSObject *object, id target,
                                                    SEL trueSelector, SEL falseSelector);
void                 ListenForPropertyChange_Hidden(NSObject *object, id target, SEL selector);