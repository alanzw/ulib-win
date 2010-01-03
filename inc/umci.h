/*
 * =====================================================================================
 *
 *       Filename:  umci.h
 *
 *    Description:  MCI utility
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

#ifndef U_MCI_H
#define U_MCI_H

#include "ulib.h"

/* The Media Control Interface (MCI) provides standard commands
 * for playing multimedia devices and recording multimedia resource files.
 */
namespace UMCI
{

ULIB_API BOOL play(LPTSTR lpstrAlias, DWORD dwFrom, DWORD dwTo);

// Plays a specified MIDI file by using MCI_OPEN and MCI_PLAY. Returns 
// as soon as playback begins. The window procedure function for the 
// specified window will be notified when playback is complete. 
// Returns 0L on success; otherwise, it returns an MCI error code.
ULIB_API DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName);

// Plays a specified waveform-audio file using MCI_OPEN and MCI_PLAY. 
// Returns when playback begins. Returns 0L on success, otherwise 
// returns an MCI error code.
ULIB_API DWORD playWAVEFile(HWND hWndNotify, LPSTR lpszWAVEFileName);

ULIB_API BOOL openCDDriver(BOOL bOpen);

class ULIB_API MP3Player
{
public: 
    MP3Player();
    ~MP3Player();
    void open(const char *filename);
    DWORD muteAll();
private:
    
};

}; // namespace UMCI

#endif // U_MCI_H

