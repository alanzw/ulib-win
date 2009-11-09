#include "playerclass.h"

// contructor
PlayerClass::PlayerClass()
{
    pGraphBuilder        = NULL;
    pMediaControl        = NULL;
    pMediaSeeking        = NULL;
    pMediaEventEx        = NULL;
    hProgressBar        = NULL;
    hOwner                = NULL;
    totalDuration        = 0;
    timeElapsed            = 0;

}


// destructor, release all resources
PlayerClass::~PlayerClass()
{
    if (pGraphBuilder)
    {
        pGraphBuilder->Release();
        pGraphBuilder = NULL;
    }
    if (pMediaControl)
    {
        pMediaControl->Release();
        pMediaControl = NULL;
    }
    if (pMediaSeeking)
    {
        pMediaSeeking->Release();
        pMediaSeeking = NULL;
    }
    if (pMediaEventEx)
    {
        // unset the notify window
        pMediaEventEx->SetNotifyWindow(NULL, 0, 0);

        //release resources
        pMediaEventEx->Release();
        pMediaEventEx = NULL;
    }

    KillTimer(hOwner, MY_TIMEREVENT);

    CoUninitialize();

}


/*
initialise the variables, allocate memory. This function also serves as reset, when the user opens another media while
a media is already playing then this will be called on the same object which serves to reset the values.
*/

void PlayerClass::Initialise(HWND owner)
{
    HRESULT hr;

    // Get an instance of the graph builder
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
                        (void**)&pGraphBuilder);

    if (!pGraphBuilder)
    {
        MessageBox(NULL, "Critical error, unable to create graph builder", "Error", MB_OK);
        ExitFromProgram();
    }

    // Get the references to interfaces
    hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
    hr = pGraphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&pMediaSeeking);
    hr = pGraphBuilder->QueryInterface(IID_IMediaEventEx, (void**)&pMediaEventEx);

    if (!pMediaControl || !pMediaSeeking || !pMediaEventEx)
    {
        MessageBox(NULL, "Critical error, error getting references", "Error", MB_OK);
        ExitFromProgram();
    }

    // hOwner stores the handle to the dialog
    hOwner = owner;

    // initialise is called everytime that a user selects a media file, so by set this button to Pause
    SetWindowText(GetDlgItem(hOwner, IDC_PLAYPAUSE), PAUSE);

    totalDuration = 0;
    timeElapsed = 0;

    // set our dialog to receive events from graph manager
    pMediaEventEx->SetNotifyWindow((OAHWND)hOwner, WM_GRAPHNOTIFY, 0);

    playing = false;


}


int PlayerClass::OpenFileDialog()
{
    OPENFILENAME ofn;

    ZeroMemory(&ofn, sizeof(ofn));

    // This is a must for GetOpenFileName to succeed! The lpstrFile member must be null terminated
    ZeroMemory(szFileName, sizeof(szFileName));

    // Initialise the OPENFILENAME structure
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hOwner;
    //ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFilter = "mp3 Files (*.mp3)\0*.mp3\0avi files (*.avi)\0*.avi\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_SIZE;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "mp3";

    // Store the handle to the progress bar here, since the file is goona be played shortly, mostly
    hProgressBar = GetDlgItem(hOwner, IDC_PROGRESSBAR);

    // open the file browser
    if (GetOpenFileName(&ofn))
    {
        HWND hWnd = NULL;

        // it is possible that the user selects another media while a media is playing currently
        // in that case stop the media first, reset the members and then play
        if (playing)
        {
            DoStop();
        }
        // this will reset
        Initialise(hOwner);

        // play the file first
        StartPlayingFile();

        // then set the name of the file that is currently playing
        hWnd = GetDlgItem(hOwner, IDC_NOWPLAYING);
        SetWindowText(hWnd, szFileName);

        // copy the ofn structure into class member variable in case we need it later
        memcpy((void *)&priFileInfo, (void *)&ofn, sizeof(ofn));
        return 1;
    }

    return 0;
}


int PlayerClass::StartPlayingFile()
{
    WCHAR wFileName[MAX_SIZE];
    char totalTime[MAX_SIZE];
    LONGLONG lDuration100NanoSecs = 0;
    long temporary;

    // convert file name to wide char
    mbstowcs(wFileName, szFileName, MAX_SIZE);

    // create the filter graph
    pGraphBuilder->RenderFile(wFileName, NULL);

    // get the time in units of 100 ns
    pMediaSeeking->GetDuration(&lDuration100NanoSecs);

    // get the total duration in seconds
    totalDuration = lDuration100NanoSecs/10000000;

    // somehow the sprintf (division and mod) doesn't work with LONGLONG so convert this to long
    temporary = (long)totalDuration;

    // normalising to 100, see note above
    step = (float)100/temporary;

    // get the total time in printable format (all divs and mods are to get hh:mm:ss
    sprintf(totalTime, "%02u:%02u:%02u", (temporary/3600)%60, (temporary/60)%60, (temporary%60));

    SetWindowText(GetDlgItem(hOwner, IDC_TOTALTIME), totalTime);

    playing = true;

    // Enable the play/pause button, since the file will now play
    EnableWindow(GetDlgItem(hOwner, IDC_PLAYPAUSE), TRUE);

    // set the range of the progress bar
    SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELONG(0, 100));//MAKELONG(0, lDurationSecs));

    // set the value by which to step
    SendMessage(hProgressBar, PBM_SETSTEP, (WPARAM)1, 0);

    // create a timer that goes off every second
    SetTimer(hOwner, MY_TIMEREVENT, 1000, (TIMERPROC)NULL);

    // play the file
    pMediaControl->Run();


    return 1;



}


int PlayerClass::DoPlayPause()
{
    if (pMediaControl != NULL)
    {
        HWND hWnd;

        // if it is playing
        if (playing)
        {
            // pause it and set the pause button to play
            pMediaControl->Pause();
            playing = false;

            hWnd = GetDlgItem(hOwner, IDC_PLAYPAUSE);
            SetWindowText(hWnd, PLAY);
        }
        else
        {
            // otherwise run it and set the play button to pause
            pMediaControl->Run();
            playing = true;

            hWnd = GetDlgItem(hOwner, IDC_PLAYPAUSE);
            SetWindowText(hWnd, PAUSE);
        }
    }

    return 1;
}

// handle the stop button
int PlayerClass::DoStop()
{
    REFERENCE_TIME rt = 0;

    if (pMediaControl != NULL)
    {
        HWND hWnd;

        // stop the media if it is playing
        if (playing)
        {
            pMediaControl->Stop();
            playing = false;

            hWnd = GetDlgItem(hOwner, IDC_PLAYPAUSE);
            SetWindowText(hWnd, PLAY);
        }

        // reset the playing to zero, so that when you play again it plays from the start
        if (pMediaSeeking)
        {
            pMediaSeeking->SetPositions(&rt, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

        }

        // on stop reset the progress bar
        SendMessage(hProgressBar, PBM_SETPOS, 0L, 0L);

        // set the time elapsed to 0
        timeElapsed = 0;

        // set the lable to zero time
        hWnd = GetDlgItem(hOwner, IDC_TIMEELAPSED);
        SetWindowText(hWnd, "00:00:00");

    }

    return 1;

}


// handle the timer
int PlayerClass::DoTimerStuff()
{

    char time[MAX_SIZE] = "";

    int pos = 0;

    // return immediately if media not playing or if the media has been played fully
    if (!playing || timeElapsed >= totalDuration)
    {
        return 0;
    }

    // increment the timeElapsed
    timeElapsed++;

    // calculate the position for the progress bar. See note in the begining of file.
    pos = (int)((float)step*(float)timeElapsed);

    SendMessage(hProgressBar, PBM_SETPOS, pos, 0L);

    sprintf(time, "%02u:%02u:%02u", (timeElapsed/3600)%60, (timeElapsed/60)%60, timeElapsed%60);

    // Set the time elapsed
    SetWindowText(GetDlgItem(hOwner, IDC_TIMEELAPSED), time);

    return 1;
}


// this will recieve the events from graph builder, see WM_GRAPHNOTIFY message in DlgDSPlayerProc
int PlayerClass::EventReceiver()
{
    HRESULT hr = S_OK;
    long evCode, param1, param2;

    //char test[MAX_SIZE];

    //MessageBox(NULL, "event received!", "info", MB_OK);

    if (!pMediaEventEx)
    {
        MessageBox(NULL, "MediaEventEx interface error", "error", MB_OK);
        return 0;
    }

    while (SUCCEEDED(pMediaEventEx->GetEvent(&evCode, &param1, &param2, 0)))
    {
        //sprintf(test, "code:%d", evCode);
        //MessageBox(NULL, test, "info", MB_OK);

        switch(evCode)
        {
            // we process only the EC_COMPLETE message which is sent when the media is finished playing
            case EC_COMPLETE:
                // Do a stop when it is finished playing
                DoStop();
                break;
        }

        hr = pMediaEventEx->FreeEventParams(evCode, param1, param2);
    }

    return 1;
}

int PlayerClass::ExitFromProgram()
{
    //TODO
    //implement all clean-ups and other stuff
    return 0;
}
