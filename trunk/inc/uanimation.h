#ifndef U_ANIMATION_H
#define U_ANIMATION_H

#include "ucontrol.h"

typedef enum tagAnimeState {
    AS_CLOSED = 0,
    AS_STOPPED,
    AS_PLAYING,
    AS_PAUSED
} AnimeSate_t;

class ULIB_API UAnimation : public UControl
{
public:
    UAnimation(HWND hParent, UINT nResource, HINSTANCE hInst);
    ~UAnimation();
    virtual BOOL create();
    //
    BOOL open(const TCHAR *filename);
    BOOL open(const UINT nResID);
    BOOL play();
    BOOL playEx(int nFrom = 0, int nTo = -1, int nReplay = -1);
    BOOL stop();
    BOOL pause();
    BOOL resume();
    BOOL close();

    //
    void setTransparent();
#if WINVER >=  0x0600
    BOOL isPlaying();
#endif

protected:
    AnimeSate_t state;
private:
};

#endif // U_ANIMATION_H

/**
 *  Animation Control Styles
 *
 *    ACS_AUTOPLAY
 *    ACS_CENTER
 *    ACS_TIMER
 *    ACS_TRANSPARENT
 *
 */
