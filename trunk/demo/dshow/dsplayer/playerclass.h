#ifndef _PLAYER_CLASS
#define _PLAYER_CLASS

#include "dsplayer.h"
#include "resource.h"

#include <dshow.h>
#include <uuids.h>
#include <Commdlg.h>

#include <Cderr.h>

#include <stdio.h>
#include <stdarg.h>

#define MAX_SIZE 256
#define MY_TIMEREVENT 9991

#define PLAY "&Play"
#define PAUSE "&Pause"

// this is the user defined message that the graph builder will send us in case of any event
#define WM_GRAPHNOTIFY WM_APP+13


class PlayerClass
{

public:
    PlayerClass();
    ~PlayerClass();

    // does some initialisation of the memeber variables
    void Initialise(HWND hWnd);

    // opens the file dialog and lets the user select a file
    int OpenFileDialog();

    int DoPlayPause();
    int DoStop();
    int DoTimerStuff();
    int EventReceiver();

private:

    int ExitFromProgram();
    int StartPlayingFile();

    IGraphBuilder *pGraphBuilder;
    IMediaControl *pMediaControl;
    IMediaSeeking *pMediaSeeking;
    IMediaEventEx *pMediaEventEx;

    // keeps track of whether the media is playing or paused/stopped
    bool playing;

    // handle to progress bar
    HWND hProgressBar;

    // handle to dialog, owner
    HWND hOwner;

    // total duration of the selected media file
    LONGLONG totalDuration;

    // keeps track of the time elapsed
    long timeElapsed;

    // stores the amount by which to step, used in the progress of the prog bar
    float step;

    // structure that contains info about the file that was opened
    OPENFILENAME priFileInfo;

    // full-path name of the file selected
    char szFileName[MAX_SIZE];

};

#endif
