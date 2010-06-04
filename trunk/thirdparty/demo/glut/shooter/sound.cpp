// Sound.cpp: implementation of the CSound class.
//
//////////////////////////////////////////////////////////////////////

#include "Sound.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////DirectSoundManager Class memeber functions///////////////

/////////////////////////////////////////////////////////////////////////////
// DirectSoundManager construction/destruction

DirectSoundManager::DirectSoundManager() 
{  
    DirectSound = NULL;
    PrimaryBuffer = NULL;

       // If Direct sound fails, just return - no sound effects will be heard

    DirectSoundCreate(NULL,&DirectSound,NULL);
    
}

DirectSoundManager::~DirectSoundManager () 
{

}

/////////////////////////////////////////////////////////////////////////////
// DirectSoundManager Initialize

BOOL DirectSoundManager::Initialize(HWND hWnd)
{
    DSBUFFERDESC BufferDesc;

   if ( DirectSound == NULL )
      return FALSE;

   // Choose priority cooperative level -- other sounds in the
   // background will still be audible, but we can change the
   // output format and perform memory management:

   if (DirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY) != DS_OK )
      return FALSE;

   BufferDesc.dwSize          = sizeof ( DSBUFFERDESC );
   BufferDesc.dwBufferBytes   = 0;
   BufferDesc.dwReserved      = 0;
   BufferDesc.lpwfxFormat     = NULL;
   BufferDesc.dwFlags         = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;

   // Create the primary sound buffer:

   if(DirectSound->CreateSoundBuffer(&BufferDesc, &PrimaryBuffer, NULL) != DS_OK )
      return FALSE;

   //If SetFormat fails, the return value may be one of the following error values:
    //DSERR_BADFORMAT 
    //DSERR_INVALIDCALL 
    //DSERR_INVALIDPARAM 
    //DSERR_OUTOFMEMORY 
    //DSERR_PRIOLEVELNEEDED 
    //DSERR_UNSUPPORTED 

    OutputFormat.wFormatTag = WAVE_FORMAT_PCM;
    OutputFormat.nChannels = 2;
    OutputFormat.nSamplesPerSec = 44100;
    OutputFormat.wBitsPerSample = 8;
    OutputFormat.nBlockAlign = OutputFormat.nChannels * OutputFormat.wBitsPerSample / 8;
    OutputFormat.nAvgBytesPerSec = OutputFormat.nBlockAlign * OutputFormat.nSamplesPerSec;

   // Set the output format:

   if(PrimaryBuffer->SetFormat(&OutputFormat) != DS_OK)
      return FALSE;

   return TRUE;
} 


/////////////////////////////////////////////////////////////////////////////
// DirectSoundManager Uninitialize

BOOL DirectSoundManager::Uninitialize()
{

    // Release all obtained interfaces:

   if ( PrimaryBuffer != NULL ) {
      PrimaryBuffer->Release ();
      PrimaryBuffer = NULL;
   }
  
   if ( DirectSound != NULL ) {
      DirectSound->Release ();
      DirectSound = NULL;
   }
   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundManager CreateSound

BOOL DirectSoundManager::CreateSound(TCHAR *FileName,DirectSoundBuffer &Buffer)
{    
    DSBUFFERDESC BufferDesc;
    WAVEFORMATEX WaveFormat;
    CWaveFile     InputWave;

    if(DirectSound == NULL)
        return FALSE;

    // Load the specified wave file:

   if ( !InputWave.Load ( FileName ) )
      return FALSE;

   // Make the format of the sound buffer to be created match the
   // format of the wave file just loaded:

   InputWave.GetWaveFormat ( WaveFormat );

   // Specify the options of the sound buffer to be created:

   BufferDesc.dwSize  = sizeof ( DSBUFFERDESC );

   // Require that the buffer have pan, volume, frequency, and
   // position notification capability (if just basic playback is
   // desired, specifiy DSBCAPS_STATIC and nothing else):

   BufferDesc.dwFlags         = DSBCAPS_CTRLPAN
                                | DSBCAPS_CTRLVOLUME
                                | DSBCAPS_CTRLFREQUENCY
                                | DSBCAPS_STATIC
                                | DSBCAPS_CTRLPOSITIONNOTIFY;
   
   BufferDesc.dwBufferBytes   = InputWave.GetDataSize ();
   BufferDesc.dwReserved      = 0;
   BufferDesc.lpwfxFormat     = &WaveFormat;

   // Create the sound buffer:

   if ( DirectSound->CreateSoundBuffer ( &BufferDesc, &Buffer.Buffer, NULL ) != DS_OK )
      return FALSE;


   // Close the input wave file so the sound buffer can access the
   // file to load the wave data:

   InputWave.Close ();

   // Have the sound buffer load its wave data:

   Buffer.Load ( FileName );

   return TRUE;

}


/////////////////////DirectSoundBuffer Class memeber functions///////////////

/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer construction/destruction

DirectSoundBuffer::DirectSoundBuffer  () {
   Buffer         = NULL;
   DataSize       = -1;
   FileName [ 0 ] = NULL;
}

DirectSoundBuffer::~DirectSoundBuffer() 
{

}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer ReleaseBuffer

void DirectSoundBuffer::ReleaseBuffer()
{
    if ( Buffer != NULL ) 
        {
         Buffer->Release();
         Buffer = NULL;
        }
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer Load

BOOL DirectSoundBuffer::Load(TCHAR *File) 
{
   CWaveFile    InputWave;
   LPVOID      BufferPointer;
   DWORD       Bytes1;

   if ( Buffer == NULL )
      return false;

   // Save the file name of the wave for restoration of the sound
   // buffer when it is lost:
   if ( File != FileName )
      lstrcpy ( FileName, File );

   // Load the specified wave file:
   if ( !InputWave.Load ( FileName ) )
      return false;

   // Store the wave format in case it's needed later:
   InputWave.GetWaveFormat ( WaveFormat );

   // Lock the sound buffer to obtain a pointer to the memory where
   // its wave data is stored:
   if ( Buffer->Lock ( 0, 0, &BufferPointer, &Bytes1, NULL, NULL,
                       DSBLOCK_ENTIREBUFFER ) != DS_OK )
      return false;

   DataSize = InputWave.GetDataSize ();

   // Store the wave data in the sound buffer:
   if ( !InputWave.GetData ( BufferPointer, DataSize ) )
      return false;

   // Unlock the sound buffer:
   if ( Buffer->Unlock ( BufferPointer, DataSize, NULL, NULL )
        != DS_OK )
      return false;

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer RestoreIfNecessary

BOOL DirectSoundBuffer::RestoreIfNecessary() 
{
   DWORD Status;

   if ( Buffer == NULL )
      return false;

   // Retrieve the status of the sound buffer:
   if ( Buffer->GetStatus ( &Status ) != DS_OK )
      return false;

   // If the sound buffer is lost, restore its memory and reload
   // its wave data:
   if ( Status & DSBSTATUS_BUFFERLOST ) {
      // Restore the buffer's memory:
      if ( Buffer->Restore () != DS_OK )
         return false;

      // Reload the wave data:
      if ( !Load ( FileName ) )
         return false;
   }

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer GetDataSize

LONG DirectSoundBuffer::GetDataSize () 
{
   if ( Buffer == NULL )
      return -1;

   return DataSize;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer GetPan

BOOL DirectSoundBuffer::GetPan(float &Pan) 
{
   LONG DSPan;

   if ( Buffer == NULL )
      return false;

   if ( Buffer->GetPan ( &DSPan ) != DS_OK )
      return false;

   Pan = float ( DSPan ) / float ( DSBPAN_RIGHT );

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer GetFrequency

BOOL DirectSoundBuffer::GetFrequency(DWORD &Frequency) 
{
   if ( Buffer == NULL )
      return false;

   if ( Buffer->GetFrequency ( &Frequency ) != DS_OK )
      return false;

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer GetVolume

BOOL DirectSoundBuffer::GetVolume(float &Volume) 
{
   LONG DSVol;

   if ( Buffer == NULL )
      return false;

   if ( Buffer->GetVolume ( &DSVol ) != DS_OK )
      return false;

   Volume = float ( fabs ( DSVol - DSBVOLUME_MIN ) / 
                    fabs ( DSBVOLUME_MAX - DSBVOLUME_MIN ) );

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer SetPan

BOOL DirectSoundBuffer::SetPan(float Pan) 
{
   LONG DSPan;

   if ( Buffer == NULL )
      return false;

   DSPan = LONG ( Pan * float ( DSBPAN_RIGHT ) );

   if ( Buffer->SetPan ( DSPan ) != DS_OK )
      return false;

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer SetFrequency

BOOL DirectSoundBuffer::SetFrequency(DWORD Frequency) 
{
   if ( Buffer == NULL )
      return false;

   if ( Buffer->SetFrequency ( Frequency ) != DS_OK )
      return false;

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer SetVolume

BOOL DirectSoundBuffer::SetVolume(float Volume) 
{
   LONG DSVol;

   if ( Buffer == NULL )
      return false;

   DSVol = LONG ( Volume *
                  float ( DSBVOLUME_MAX - DSBVOLUME_MIN ) )
           + DSBVOLUME_MIN;

   if ( Buffer->SetVolume ( DSVol ) != DS_OK )
      return false;

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer GetWaveFormat

BOOL DirectSoundBuffer::GetWaveFormat(WAVEFORMATEX &Out) 
{
   if ( Buffer == NULL )
      return false;

   Out = WaveFormat;

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer Stop

BOOL DirectSoundBuffer::Stop() 
{
   if ( Buffer == NULL )
      return false;

   if ( Buffer->Stop () != DS_OK )
      return false;

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer IsPlaying

BOOL DirectSoundBuffer::IsPlaying() 
{
   DWORD Status;

   if ( Buffer == NULL )
      return false;

   if ( Buffer->GetStatus ( &Status ) != DS_OK )
      return false;

   return ( Status & DSBSTATUS_PLAYING );
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer Play

BOOL DirectSoundBuffer::Play(bool Loop) 
{
   if ( Buffer == NULL )
      return false;

   // Restore lost memory if necessary before attempting to play
   // the sound:
   if ( !RestoreIfNecessary () )
      return false;

   if ( Loop )
      Buffer->Play ( 0, 0, DSBPLAY_LOOPING );
   else Buffer->Play ( 0, 0, 0 );

   return true;
}


/////////////////////////////////////////////////////////////////////////////
// DirectSoundBuffer playSoundArray

void playSoundArray(DirectSoundBuffer sound[], int num)
{
    BOOL bFoundEmptySound = FALSE;
    for(int index = 0; index < num; index++)
    {
        if(sound[index].IsPlaying ()  == false)
        {
            sound[index].Play(0);
            bFoundEmptySound = TRUE;
        }
        if(bFoundEmptySound)
            break;
    }
}

