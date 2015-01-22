//
//  GeneralError.h
//
//  An error class optionally containing an NSError instance and additional information
//  

#import <Foundation/Foundation.h>

typedef enum { ErrorType_Success, ErrorType_Warning, ErrorType_Error } ErrorType;

@interface GeneralError : NSObject

@property                     ErrorType type;
@property                     int       code;
@property (nonatomic, retain) NSString *identifier;
@property (nonatomic, retain) NSString *message;

@property (nonatomic, retain) NSError  *nserror;

- (id)initWithIdentifier:(NSString *)identifier code:(int)code message:(NSString *)message;

@end
