// Wave.h: interface for the CWave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVE_H__19C39621_A370_11D4_A3B0_0050DA731472__INCLUDED_)
#define AFX_WAVE_H__19C39621_A370_11D4_A3B0_0050DA731472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaveFile {
   protected:
      HMMIO            InputHandle;
      WAVEFORMATEX     WaveFormat;
      MMCKINFO         DataChunk, ParentChunk;      
      bool             Loaded;

      bool CleanUp ();
   public:
      CWaveFile ();
      ~CWaveFile ();

      bool Load ( TCHAR *FileName );

      WORD GetFormat ();
      WORD GetBlockAlign ();
      WORD GetChannelCount ();
      WORD GetBitsPerSample ();
      
      DWORD GetSamplesPerSecond ();
      DWORD GetAverageBytesPerSecond ();

      DWORD GetDataSize ();

      bool GetData ( void *Buffer, LONG Bytes );

      bool GetWaveFormat ( WAVEFORMATEX &Format );

      bool Close ();

      operator WAVEFORMATEX ();
      operator HMMIO ();
};

#endif // !defined(AFX_WAVE_H__19C39621_A370_11D4_A3B0_0050DA731472__INCLUDED_)
