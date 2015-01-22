//
//  PlatformUtil.m
//

#import "PlatformUtil.h"
#import "SysUtil.h"
#import <Foundation/Foundation.h>
#import <mach/mach.h>
#import <mach/mach_host.h>
#import <sys/sysctl.h>

#import <UIKit/UIKit.h>

void PrintPlatformInfo(void)
{
    NSLog(@"-- PLATFORM INFO --");
    NSLog(@"UDID: %@", GetPlatformUniqueIdentifier());
    NSLog(@"Name: %@", GetPlatformDeviceName());
    NSLog(@"System Name: %@", GetPlatformSystemName());
    NSLog(@"System Version: %@", GetPlatformSystemVersion());
    NSLog(@"Model: %@", GetPlatformModel());
    NSLog(@"Machine: %@", GetPlatformMachine());
    NSLog(@"-------------------");
}

NSString *GetPlatformUniqueIdentifier(void)
{
	return [NSString stringWithFormat:@"DEPRECATED-NOW-USING-%@", GetUserUniqueUDID()];//[[UIDevice currentDevice] uniqueIdentifier];
}

NSString *GetPlatformDeviceName(void)
{
	return [[UIDevice currentDevice] name];
}

NSString *GetPlatformSystemName(void)
{
	return [[UIDevice currentDevice] systemName];
}

NSString *GetPlatformSystemVersion(void)
{
	return [[UIDevice currentDevice] systemVersion];
}

int GetPlatformSystemVersionMajor(void)
{
    NSString *version = [[UIDevice currentDevice] systemVersion];
    if (!version)
        return -1;
    NSArray *c = [version componentsSeparatedByString:@"."];
    NSString *major = (c && c.count) ? [c objectAtIndex:0] : nil;
    int value = major ? [major intValue] : -1;
    return value >= 1 ? value : -1;
}

NSString *GetPlatformModel(void)
{
	return [[UIDevice currentDevice] model];
}

NSString *GetPlatformMachine(void)
{
    static char machine[1024] = "";
    size_t size = 0;  
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);  
    sysctlbyname("hw.machine", machine, &size, NULL, 0);  
    return [NSString stringWithUTF8String:machine];
}

NSString *GetPlatformLocalizedModel()
{
	return [[UIDevice currentDevice] localizedModel];
}

BOOL IsSimulator()
{
	static int is_simulator = -1;
	if (is_simulator == -1)
	    is_simulator = [GetPlatformModel() rangeOfString:@"imulator"].location != NSNotFound;
	return is_simulator;
}

BOOL IsIPad()
{
	static int is_ipad = -1;
	if (is_ipad == -1)
	{
        NSString *machine = GetPlatformMachine(), *platform = GetPlatformModel();
	    is_ipad = ([machine rangeOfString:@"iPad"].location != NSNotFound) ||
	              ([platform rangeOfString:@"iPad"].location != NSNotFound);
	}
	return (BOOL)is_ipad;
}
