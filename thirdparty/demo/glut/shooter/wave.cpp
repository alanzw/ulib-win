// Wave.cpp: implementation of the CWave class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "wave.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaveFile construction/destruction

CWaveFile::CWaveFile () 
{
   Loaded = false;
}

CWaveFile::~CWaveFile () 
{
   CleanUp ();
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile CleanUp

bool CWaveFile::CleanUp () 
{
   // If the RIFF file has been opened, close it and set the Loaded
   // flag to false:
   if ( Loaded ) {
      if ( mmioClose ( InputHandle, 0 ) != 0 )
         return false;
      Loaded = false;
   }
   return true;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile Load

bool CWaveFile::Load ( TCHAR *FileName ) 
{
   MMCKINFO InputChunk;

   const DWORD WaveFCC = mmioFOURCC ( 'W', 'A', 'V', 'E' ),
               MinSize = sizeof WaveFormat - 
                         sizeof WaveFormat.cbSize;

   // Close the RIFF file if one was previously opened using this 
   // object:
   if ( !CleanUp () )
      return false;

   // Attempt to open the specified RIFF file:
   InputHandle = mmioOpen ( FileName, NULL, 
                            MMIO_ALLOCBUF | MMIO_READ );

   if ( InputHandle == NULL ) {
      mmioClose ( InputHandle, 0 );
      return false;
   }

   ZeroMemory ( &ParentChunk, sizeof ParentChunk );

   // Descend into the parent chunk:
   if ( mmioDescend ( InputHandle, &ParentChunk, NULL, 0 ) != 0 ) {
      mmioClose ( InputHandle, 0 );
      return false;
   }

   // Make sure the parent chunk is actually a RIFF chunk and that
   // it contains wave data:
   if ( ( ParentChunk.ckid != FOURCC_RIFF ) 
        || ( ParentChunk.fccType != WaveFCC ) ) {
      mmioClose ( InputHandle, 0 );
      return false;
   }
   
   ZeroMemory ( &InputChunk, sizeof InputChunk );
   InputChunk.ckid = mmioFOURCC ( 'f', 'm', 't', ' ' );

   // Descend into the 'fmt ' chunk, where the attributes of the
   // wave data are stored:
   if ( mmioDescend ( InputHandle, &InputChunk, &ParentChunk, 
                      MMIO_FINDCHUNK ) != 0 ) {
      mmioClose ( InputHandle, 0 );
      return false;
   }

   // Make sure the format chunk contains sufficient information:
   if ( InputChunk.cksize < MinSize ) {
      mmioClose ( InputHandle, 0 );
      return false;
   }
   
   // Read the format chunk's data:
   if ( mmioRead ( InputHandle, ( char * ) &WaveFormat, 
                   sizeof WaveFormat ) < MinSize ) {
      mmioClose ( InputHandle, 0 );
      return false;
   }

   // Seek to the beginning of the parent chunk to begin a search:
   if ( mmioSeek ( InputHandle, ParentChunk.dwDataOffset + 
                sizeof ( FOURCC ), SEEK_SET ) == -1 ) {
      mmioClose ( InputHandle, 0 );
      return false;
   }

   ZeroMemory ( &InputChunk, sizeof InputChunk );
   InputChunk.ckid = mmioFOURCC ( 'd', 'a', 't', 'a' );

   // Descend into the 'data' chunk:
   if ( mmioDescend ( InputHandle, &InputChunk, &ParentChunk, 
                      MMIO_FINDCHUNK ) != 0 ) {
      mmioClose ( InputHandle, 0 );
      return false;
   }


   // Save the 'data' chunk information for later use:
   DataChunk = InputChunk;
   
   // Record that the wave file was successfully loaded:
   Loaded = true;

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile GetFormat

WORD CWaveFile::GetFormat () 
{
   // The format for PCM wave files is WAVE_FORMAT_PCM

   if ( !Loaded )
      return 0;

   return WaveFormat.wFormatTag;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile GetBlockAlign

WORD CWaveFile::GetBlockAlign () 
{
   if ( !Loaded )
      return 0;

   return WaveFormat.nBlockAlign;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile GetChannelCount

WORD CWaveFile::GetChannelCount () 
{
   if ( !Loaded )
      return 0;

   return WaveFormat.nChannels;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile GetBitsPerSample

WORD CWaveFile::GetBitsPerSample () 
{
   if ( !Loaded )
      return 0;

   return WaveFormat.wBitsPerSample;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile GetSamplesPerSecond

DWORD CWaveFile::GetSamplesPerSecond () 
{
   if ( !Loaded )
      return 0;

   return WaveFormat.nSamplesPerSec;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile GetAverageBytesPerSecond

DWORD CWaveFile::GetAverageBytesPerSecond () 
{
   if ( !Loaded )
      return 0;

   return WaveFormat.nAvgBytesPerSec;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile GetDataSize

DWORD CWaveFile::GetDataSize ()
{
   if ( !Loaded )
      return 0;

   return DataChunk.cksize;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile GetData

bool CWaveFile::GetData ( void *Buffer, LONG Bytes ) 
{
   if ( !Loaded )
      return false;

   // Read the wave data (note that buffered IO, which the mmio*
   // functions support, would be faster, but most games
   // will read all wave files at once, during the initialization
   // phase, when speed is not an issue):
   if ( mmioRead ( InputHandle, ( char * ) Buffer, Bytes )
        != Bytes )
      return false;

   return true;
}

/////////////////////////////////////////////////////////////////////////////
// CWaveFile GetWaveFormat

bool CWaveFile::GetWaveFormat ( WAVEFORMATEX &Format ) 
{
   if ( !Loaded )
      return false;

   Format = WaveFormat;

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// CWaveFile Close

bool CWaveFile::Close () 
{
   return CleanUp ();
}

/////////////////////////////////////////////////////////////////////////////
// CWaveFile operators

CWaveFile::operator WAVEFORMATEX () 
{
   return WaveFormat;
}

CWaveFile::operator HMMIO () 
{
   return InputHandle;
}
