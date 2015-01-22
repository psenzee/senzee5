//
//  SoundUtil.m
//

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "SoundUtil.h"

unsigned long CreateSystemSoundWithMp3File(const char *filename)
{
	CFURLRef url = NULL;
    NSString *path = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", filename] ofType:@""];
    NSLog(@"SOUND PATH: %@", path);
    if (!path)
        return 0;
    
	url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, (CFStringRef)path, kCFURLPOSIXPathStyle, false);
    SystemSoundID soundid = 0;
	OSStatus err = AudioServicesCreateSystemSoundID(url, &soundid);
	if (err == 0)
    {
        // err = AudioServicesAddSystemSoundCompletion(soundid, NULL, NULL, SoundComplete, self);
    }
    if (err)
    {
        NSLog(@"create sound error %d", (int)err);
        soundid = 0;
    }
    CFRelease(url);
    return (unsigned long)soundid;
}

void PlaySystemSound(unsigned long soundid)
{
    if (soundid > 0)
        AudioServicesPlaySystemSound((SystemSoundID)soundid);
}