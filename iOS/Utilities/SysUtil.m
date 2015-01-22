//
//  SysUtil.m
//

#import "SysUtil.h"
#import <Foundation/Foundation.h>
#import <mach/mach.h>
#import <mach/mach_host.h>
#import <sys/sysctl.h>

#import <UIKit/UIKit.h>

void MainThreadCheck(NSString *where)
{
    if (![NSThread isMainThread])
        NSLog(@"*NOT* MAIN THREAD IN %@", where);
    else
        NSLog(@"*IS* MAIN THREAD IN %@", where);
}

void ReleaseOnMainThread(NSObject *object)
{
    if (!object)
        return;
    if ([NSThread isMainThread])
        [object release];
    else
    {
        [object performSelectorOnMainThread:@selector(release) withObject:nil waitUntilDone:YES];
    }
}

@interface DelayedSelector : NSObject
{
    id              _this;
	SEL             _selector;
    NSObject       *_object;
    NSTimeInterval  _delay;
}

- (id)initWithSelector:(SEL)selector withSelf:(id)this withObject:object delay:(NSTimeInterval)delay;
- (void)run;

@end

@implementation DelayedSelector

- (id)initWithSelector:(SEL)selector withSelf:(id)this withObject:object delay:(NSTimeInterval)delay
{
    self = [super init];
    _this = this;
    [this retain];
    _selector = selector;
    _object = object;
    [object retain];
    _delay = delay;
    return self;
}

- (void)run
{
    [_this performSelector:_selector withObject:_object afterDelay:_delay];
    [_this release];
    [_object release];
}

@end

void PerformBlockOnMainThread(void (^block)())
{
    dispatch_async(dispatch_get_main_queue(), block);
}

void PerformOnMainThread(id object, SEL selector, id withObject, BOOL wait)
{
	if (object && [object respondsToSelector:selector])
	{
        if ([NSThread isMainThread])
            [object performSelector:selector withObject:withObject];
        else
            [object performSelectorOnMainThread:selector
                                     withObject:withObject waitUntilDone:wait];
	}
}

void PerformOnMainThreadAfterDelay(id object, SEL selector, id withObject, NSTimeInterval delay)
{
	if (object && [object respondsToSelector:selector])
	{
        if ([NSThread isMainThread])
        {
            [object performSelector:selector withObject:withObject afterDelay:delay];
        }
        else
        {
            DelayedSelector *ds = [[DelayedSelector alloc] initWithSelector:selector withSelf:object withObject:withObject delay:delay];
            [ds performSelectorOnMainThread:@selector(run) withObject:nil waitUntilDone:NO];
            [ds release];
        }
	}
}

BOOL PerformIfResponds(id object, SEL selector)
{
    if (object && selector && [object respondsToSelector:selector])
    {
        [object performSelector:selector];
        return YES;
    }
    return NO;
}

NSString *GenerateUUID(void)
{
    NSString *uuidString = nil;
    CFUUIDRef uuid = CFUUIDCreate(NULL);
    if (uuid)
    {
        uuidString = (NSString *)CFUUIDCreateString(NULL, uuid);
        CFRelease(uuid);
    }
    return uuidString;
}

NSString *GetUserUniqueUDID(void)
{
    NSString *key = @"USERUNIQUEUUID";
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString *uuid = [defaults objectForKey:key];
    if (uuid == nil)
    {
        uuid = GenerateUUID();
        [defaults setObject:uuid forKey:key];
        [defaults synchronize];
    }
    return uuid;
}

NSString *GetUserUniqueUUID(void)
{
    return GetUserUniqueUDID();
}

void GoToUrlInBrowser(NSString *url)
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

void ReadInfoDictionary(NSMutableDictionary *properties)
{
	NSDictionary *dict = [[NSBundle mainBundle] infoDictionary];
	for (NSString *key in [dict allKeys])
		[properties setObject:[dict objectForKey:key] forKey:key];
}

void ReleaseAndRemoveAllObjects(NSMutableArray *array)
{
    for (NSObject *object in array)
        [object release];
    [array removeAllObjects];
}

void PrintRefCountForDictionary(NSMutableDictionary *dict)
{
    if (dict)
    {
        printf("\nREFCOUNTS:");
        for (NSObject *object in [dict allValues])
            printf("%d,", [object retainCount]);
        printf("\n");
    }
}

void ReleaseAndRemoveAllObjectsFromDictionary(NSMutableDictionary *dict)
{
    for (NSObject *object in [dict allValues])
        [object release];
    [dict removeAllObjects];
}

void PrintFreeMemory()
{
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;
    
    host_port = mach_host_self();
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize);        
    
    vm_statistics_data_t vm_stat;
    
    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS)
        NSLog(@"Failed to fetch vm statistics");
    
    /* Stats in bytes */ 
    natural_t mem_used = (vm_stat.active_count +
                          vm_stat.inactive_count +
                          vm_stat.wire_count) * pagesize;
    natural_t mem_free = vm_stat.free_count * pagesize;
    natural_t mem_total = mem_used + mem_free;
    double mb = 1.0 / (1024.0 * 1024.0);
    NSLog(@"used: %u (%.2fMB) free: %u (%.2fMB) total: %u (%.2fMB)", mem_used, mem_used * mb, mem_free, mem_free * mb, mem_total, mem_total * mb);
}

double GetFreeMemory()
{
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;
    
    host_port = mach_host_self();
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize);        
    
    vm_statistics_data_t vm_stat;
    
    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS)
        NSLog(@"Failed to fetch vm statistics");
    
    /* Stats in bytes */ 
//    natural_t mem_used = (vm_stat.active_count +
  //                        vm_stat.inactive_count +
    //                      vm_stat.wire_count) * pagesize;
    natural_t mem_free = vm_stat.free_count * pagesize;
  //  natural_t mem_total = mem_used + mem_free;
    double mb = 1.0 / (1024.0 * 1024.0);
    return mem_free * mb;
}

@interface GeneralPropertyListener : NSObject
{
    id  _target;
    SEL _changeSelector;
}

@property (nonatomic, retain) id  target;
@property                     SEL changeSelector;

- (id)initWithTarget:(id)target changeSelector:(SEL)changeSelector;
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context;
@end

@implementation GeneralPropertyListener

@synthesize target = _target;
@synthesize changeSelector = _changeSelector;

- (id)initWithTarget:(id)target changeSelector:(SEL)changeSelector
{
    self.target = target;
    self.changeSelector = changeSelector;
    return self;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if (_target && change)
    {
        NSNumber *option = (NSNumber *)[change objectForKey:@"new"];
        if (option)
        {
            if (self.changeSelector)
                [_target performSelector:self.changeSelector withObject:option];
        }
    }
}

@end

void ListenForPropertyChange(NSString *property, NSObject *object, id target, SEL changeSelector)
{
    GeneralPropertyListener *listener = [[GeneralPropertyListener alloc] initWithTarget:target changeSelector:changeSelector];
    [object addObserver:listener forKeyPath:property options:(NSKeyValueObservingOptionNew) context:NULL];
}

@interface BooleanPropertyListener : NSObject
{
    id  _target;
    SEL _trueSelector;
    SEL _falseSelector;
}

@property (nonatomic, retain) id  target;
@property                     SEL trueSelector;
@property                     SEL falseSelector;

- (id)initWithTarget:(id)target trueSelector:(SEL)trueSelector falseSelector:(SEL)falseSelector;
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context;

@end

@implementation BooleanPropertyListener

@synthesize target = _target;
@synthesize trueSelector = _trueSelector;
@synthesize falseSelector = _falseSelector;

- (id)initWithTarget:(id)target trueSelector:(SEL)trueSelector falseSelector:(SEL)falseSelector
{
    self.target = target;
    self.trueSelector = trueSelector;
    self.falseSelector = falseSelector;
	return self;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if (_target && change)
    {
        NSNumber *option = (NSNumber *)[change objectForKey:@"new"];
        if (option)
        {
            SEL selector = (option.boolValue ? _trueSelector : _falseSelector);
            if (selector)
                [_target performSelector:selector withObject:option];
        }
    }
}

@end

void ListenForBooleanPropertyChange(NSString *property, NSObject *object, id target,
                                    SEL trueSelector, SEL falseSelector)
{
    BooleanPropertyListener *listener = [[BooleanPropertyListener alloc] initWithTarget:target trueSelector:trueSelector falseSelector:falseSelector];
    [object addObserver:listener forKeyPath:property options:(NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld) context:NULL];
}

void ListenForPropertyChange_Hidden(NSObject *object, id target, SEL selector)
{
    ListenForBooleanPropertyChange(@"hidden", object, target, selector, selector);
}

