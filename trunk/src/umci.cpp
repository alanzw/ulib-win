/*
 * =====================================================================================
 *
 *       Filename:  umci.cpp
 *
 *    Description:  MCI utility implement
 *
 *        Version:  1.0
 *        Created:  2009-8-25 2:00:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "umci.h"

namespace UMCI
{

BOOL play(LPTSTR lpstrAlias, DWORD dwFrom, DWORD dwTo)
{
    TCHAR achCommandBuff[128];
    int result;
    MCIERROR err;

    // Form the command string.
    result = _stprintf( achCommandBuff, TEXT("play %s from %u to %u"),
                        lpstrAlias, dwFrom, dwTo );

    if (result == -1)
    {
        return FALSE;
    }

    // Send the command string.
    err = mciSendString(achCommandBuff, 0, 0, 0);
    if (err != 0)
    {
        return FALSE;
    }

    return TRUE;
}

DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName)
{
    UINT wDeviceID;
    DWORD dwReturn;
    MCI_OPEN_PARMS mciOpenParms;
    MCI_PLAY_PARMS mciPlayParms;
    MCI_STATUS_PARMS mciStatusParms;
    MCI_SEQ_SET_PARMS mciSeqSetParms;

    // Open the device by specifying the device and filename.
    // MCI will attempt to choose the MIDI mapper as the output port.
    mciOpenParms.lpstrDeviceType = "sequencer";
    mciOpenParms.lpstrElementName = lpszMIDIFileName;
    if (dwReturn = mciSendCommand(0, MCI_OPEN,
        MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
        (DWORD)(LPVOID) &mciOpenParms))
    {
        // Failed to open device. Don't close it; just return error.
        return (dwReturn);
    }

    // The device opened successfully; get the device ID.
    wDeviceID = mciOpenParms.wDeviceID;

    // Check if the output port is the MIDI mapper.
    mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_STATUS,
        MCI_STATUS_ITEM, (DWORD)(LPVOID) &mciStatusParms))
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, 0);
        return (dwReturn);
    }

    // The output port is not the MIDI mapper.
    // Ask if the user wants to continue.
    if ( mciStatusParms.dwReturn != MIDI_MAPPER)
    {
        if ( MessageBox(hWndNotify,
              "The MIDI mapper is not available. Continue?",
              "", MB_YESNO) == IDNO)
        {
            // User does not want to continue. Not an error;
            // just close the device and return.
            mciSendCommand(wDeviceID, MCI_CLOSE, 0, 0);
            return (0L);
        }
    }

    // Begin playback. The window procedure function for the parent
    // window will be notified with an MM_MCINOTIFY message when
    // playback is complete. At this time, the window procedure closes
    // the device.
    mciPlayParms.dwCallback = (DWORD) hWndNotify;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY,
        (DWORD)(LPVOID) &mciPlayParms))
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, 0);
        return (dwReturn);
    }

    return 0L;
}

DWORD playWAVEFile(HWND hWndNotify, LPSTR lpszWAVEFileName)
{
    UINT wDeviceID;
    DWORD dwReturn;
    MCI_OPEN_PARMS mciOpenParms;
    MCI_PLAY_PARMS mciPlayParms;

    // Open the device by specifying the device and filename.
    // MCI will choose a device capable of playing the specified file.

    mciOpenParms.lpstrDeviceType = "waveaudio";
    mciOpenParms.lpstrElementName = lpszWAVEFileName;
    if (dwReturn = mciSendCommand(0, MCI_OPEN,
       MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
       (DWORD)(LPVOID) &mciOpenParms))
    {
        // Failed to open device. Don't close it; just return error.
        return (dwReturn);
    }

    // The device opened successfully; get the device ID.
    wDeviceID = mciOpenParms.wDeviceID;

    // Begin playback. The window procedure function for the parent
    // window will be notified with an MM_MCINOTIFY message when
    // playback is complete. At this time, the window procedure closes
    // the device.

    mciPlayParms.dwCallback = (DWORD) hWndNotify;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY,
        (DWORD)(LPVOID) &mciPlayParms))
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, 0);
        return (dwReturn);
    }

    return 0L;
}

BOOL openCDDriver(BOOL bOpen)
{
    DWORD dwReturn;
    MCI_OPEN_PARMS mciOpenParms;
    UINT wDeviceID;
    BOOL bSucc = TRUE;

    // Open the device by specifying the device name.
    mciOpenParms.lpstrDeviceType = "cdaudio";
    if  (dwReturn = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpenParms))
    {
        // Failed to open device.
        // Don't close device; just return error.
        return FALSE;
    }

    // The device opened successfully
    // Get the device ID.
    wDeviceID = mciOpenParms.wDeviceID;
    MCI_SET_PARMS mciSetParms;
    if (bOpen)
    {
       if(dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_DOOR_OPEN, (DWORD)(LPSTR)&mciSetParms))
       {
            //ErrorProc(dwReturn);
            bSucc   =   FALSE;
        }
    }
    else
    {
        if(dwReturn = mciSendCommand(wDeviceID, MCI_SET, MCI_SET_DOOR_CLOSED, (DWORD)(LPSTR)&mciSetParms))
        {
            //ErrorProc(dwReturn);
            bSucc   =   FALSE;
        }
    }
    mciSendCommand(wDeviceID, MCI_CLOSE, 0, 0);
    return   bSucc;
}


DWORD MP3Player::muteAll()
{
    return mciSendString("setaudio MediaFile off", 0, 0, 0);
}


}; // namespace UMCI

