//
//  PlatformUtil.h
//
//  A set of convenience functions for querying platform information.

#import <Foundation/Foundation.h>

NSString            *GetPlatformUniqueIdentifier(void);
NSString            *GetPlatformDeviceName(void);
NSString            *GetPlatformSystemName(void);
NSString            *GetPlatformSystemVersion(void);
int                  GetPlatformSystemVersionMajor(void);
NSString            *GetPlatformModel(void);
NSString            *GetPlatformMachine(void);
NSString            *GetPlatformLocalizedModel(void);
void                 PrintPlatformInfo(void);
BOOL                 IsSimulator(void);
BOOL                 IsIPad(void);