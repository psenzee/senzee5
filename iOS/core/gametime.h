#ifndef _GAMETIME_H#define _GAMETIME_H#ifdef __cplusplusextern "C" {#endiftypedef struct{    double elapsed;    } GameTime;    inline GameTime MakeGameTime(double ms){    GameTime time;    time.elapsed = ms;    return time;}#ifdef __cplusplus}#endif    #endif // _GAMETIME_H