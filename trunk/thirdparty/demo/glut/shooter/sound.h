// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUND_H__19C39622_A370_11D4_A3B0_0050DA731472__INCLUDED_)
#define AFX_SOUND_H__19C39622_A370_11D4_A3B0_0050DA731472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>    // Standard Windows header
#include <mmsystem.h>    // Multimedia functions and structures
#include <direct.h>        // DirectX defines (many of dsound structures)
#include "dsound.h"        // Direct sound Header
#include <stdio.h>        // File I/O routines
#include "wave.h"        // Wave format


class DirectSoundBuffer;

class DirectSoundManager
{
   protected:
      LPDIRECTSOUND           DirectSound;
      LPDIRECTSOUNDBUFFER     PrimaryBuffer;

      WAVEFORMATEX           OutputFormat;
   public:
      DirectSoundManager  ();
      ~DirectSoundManager ();

      BOOL CreateSound ( TCHAR *FileName,
                         DirectSoundBuffer &Buffer );


      BOOL Initialize(HWND hWnd);
      BOOL Uninitialize ();

};

class DirectSoundBuffer
{
   protected:
      LPDIRECTSOUNDBUFFER Buffer;

      TCHAR FileName [ MAX_PATH ];
      LONG DataSize;
      WAVEFORMATEX WaveFormat;

      BOOL Load ( TCHAR *File );

      BOOL RestoreIfNecessary ();

      friend class DirectSoundManager;
   public:
      DirectSoundBuffer  ();
      ~DirectSoundBuffer ();

      void ReleaseBuffer();
      BOOL GetPan ( float &Pan );
      BOOL GetFrequency ( DWORD &Frequency );
      BOOL GetVolume ( float &Volume );

      BOOL SetPan ( float Pan );
      BOOL SetFrequency ( DWORD Frequency );
      BOOL SetVolume ( float Volume );

      LONG GetDataSize ();
      BOOL GetWaveFormat ( WAVEFORMATEX &Out );

      BOOL Stop ();
      BOOL IsPlaying ();
      BOOL Play ( bool Loop = false );

};

// Are one and only function prototype

void playSoundArray(DirectSoundBuffer sound[], int num);

#endif // !defined(AFX_SOUND_H__19C39622_A370_11D4_A3B0_0050DA731472__INCLUDED_)
