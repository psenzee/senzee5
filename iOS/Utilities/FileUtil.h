//
//  FileUtil.h
//

#import <Foundation/Foundation.h>

BOOL          IsURL(NSString *path);

long long     GetFileSize(NSString *path);
NSString     *GetPathFor(NSString *filename, BOOL readOnlyPath);
NSURL        *GetURLFor(NSString *filename, BOOL readOnlyPath);
NSArray      *GetDirectory(NSString *path, BOOL readOnlyPath);

NSData       *ReadURL(NSString *path);
NSData       *ReadFileForName(NSString *filename, BOOL readOnlyPath);
NSData       *ReadFile(NSString *filename);

void          WriteStringToFile(NSString *filename, NSString *data);