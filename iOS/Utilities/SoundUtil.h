//
//  SoundUtil.h
//

#ifdef __cplusplus
extern "C" {
#endif

unsigned long CreateSystemSoundWithMp3File(const char *filename);
void          PlaySystemSound(unsigned long soundid);
    
#ifdef __cplusplus
}
#endif
