//
//  FileUtil.m
//

#import "FileUtil.h"
#import "DataUtil.h"
#import "StringUtil.h"
#import <Foundation/Foundation.h>

BOOL IsURL(NSString *path)
{
    return path && [path rangeOfString:@"://"].location != NSNotFound;
}

long long GetFileSize(NSString *path)
{
    return [[[NSFileManager defaultManager] attributesOfItemAtPath:path error:nil][NSFileSize] longLongValue];
}

NSString *GetPathFor(NSString *filename, BOOL readOnlyPath)
{
    if (!filename)
        return nil;
    if (IsURL(filename))
        return filename;
    NSString *path = [[NSBundle mainBundle] resourcePath];
    if (!readOnlyPath) 
        path = [NSSearchPathForDirectoriesInDomains(
                    NSDocumentDirectory, NSUserDomainMask, YES) 
                    objectAtIndex:0];
    return [path stringByAppendingFormat:@"/%@", filename];
}

NSURL *GetURLFor(NSString *filename, BOOL readOnlyPath)
{
    return [NSURL URLWithString:GetPathFor(filename, readOnlyPath)];
}

NSArray *GetDirectory(NSString *path, BOOL readOnlyPath)
{
    NSURL *url = GetURLFor(path, readOnlyPath);
    NSError *error = nil;
    NSArray *properties = [NSArray arrayWithObjects: NSURLLocalizedNameKey,
                           NSURLCreationDateKey, NSURLLocalizedTypeDescriptionKey, nil];
    NSArray *array = [[NSFileManager defaultManager]
                      contentsOfDirectoryAtURL:url
                      includingPropertiesForKeys:properties
                      options:(NSDirectoryEnumerationSkipsHiddenFiles)
                      error:&error];
    if (array == nil)
    {
        // Handle the error
    }
    return array; // can be nil if error
}
				 
NSData *ReadURL(NSString *path)
{	
	if (!path || ![path length])
		return nil;
	NSURL *url = nil;
	if (!IsURL(path))
		url = [NSURL fileURLWithPath:path];
	else url = [NSURL URLWithString:path];
	return AllocateDataWithContentsOfUrl(url);
}
				 
NSData *ReadFileForName(NSString *filename, BOOL readOnlyPath)
{
	return ReadURL(GetPathFor(filename, readOnlyPath));
}

NSData *ReadFile(NSString *filename)
{
	NSData *data = ReadFileForName(filename, NO);
	return data != nil ? data : ReadFileForName(filename, YES);
}

void WriteStringToFile(NSString *filename, NSString *data)
{
    filename = GetPathFor(filename, NO);
    [data writeToFile:filename atomically:NO 
         encoding:NSUTF8StringEncoding error:nil];	
}


NSString *GetMimeTypeFor(NSString *filename)
{
	NSString *ext = [filename pathExtension]; // need to handle URLs properly too
	NSString *mime = @"text/html";
	// add more as your heart desires
	if      ([ext isEqualToString:@"png"]) mime = @"image/png";
	else if ([ext isEqualToString:@"jpg"]) mime = @"image/jpeg";
	else if ([ext isEqualToString:@"txt"]) mime = @"text/plain";	
	else if ([ext isEqualToString:@"xml"]) mime = @"text/xml";
	else if ([ext isEqualToString:@"css"]) mime = @"text/css";
	else if ([ext isEqualToString:@"js"])  mime = @"text/javascript";		
	return mime;
}

NSString *ReadURLAsString(NSString *filename)
{
	NSData *data = ReadURL(filename);
    if (!data)
        return nil;
    NSString *str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    [data release];
    return str;
}

NSInputStream *OpenURLStream(NSString *path)
{
	if (!path || ![path length])
		return nil;
	NSURL *url = nil;
	if (!IsURL(path))
		url = [NSURL fileURLWithPath:path];
	else url = [NSURL URLWithString:path];
	return [[NSInputStream alloc] initWithURL:url];
}

NSString *ReadFileAsString(NSString *filename)
{
	NSData *data = ReadFile(filename);
    if (!data)
        return nil;
    NSString *str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    [data release];
    return str;
}

NSArray *ReadFileAsLines(NSString *filename, NSString *delimiter)
{
	NSString *str = ReadFileAsString(filename);
    if (!str)
        return nil;
    NSArray *a = [[NSArray alloc] initWithArray:[str componentsSeparatedByString:delimiter]];
    [str release];
    return a;
}

NSDictionary *ReadFileAsDictionary(NSString *filename, NSString *lineDelimiter, NSString *keyValueDelimiter)
{
	NSArray *lines = ReadFileAsLines(filename, lineDelimiter), *segments = nil;
    if (!lines)
        return nil;
    NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
    for (NSString *s in lines)
    {
        segments = FirstComponentsSeparatedByString(s, keyValueDelimiter);
        if (segments.count >= 2)
            [dict setObject:[segments objectAtIndex:1] forKey:[segments objectAtIndex:0]];
    }
    NSDictionary *result = [[NSDictionary alloc] initWithDictionary:dict];
    [dict release];
    return result;
}