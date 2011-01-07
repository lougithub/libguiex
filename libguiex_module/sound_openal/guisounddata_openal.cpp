/** 
* @file guisounddata.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/

//============================================================================//
// include
//============================================================================//
#include "guisounddata_openal.h"
#include "guioggfunc.h"
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiscenemanager.h>

#if GUIEX_PLATFORM_WIN32
#include <windows.h>
#endif
//============================================================================//
// function
//============================================================================//
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISoundData_openal::CGUISoundData_openal( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUISoundData( rName, rSceneName )
		,m_strPath( rPath )
		,m_nSourceId( 0 )
		,m_nBufferId( 0 )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData_openal::~CGUISoundData_openal()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundData_openal::DoLoad() const
	{
		// identify file type by extension
		CGUIString strExt;
		size_t pos = m_strPath.find_last_of(".");
		if( pos != CGUIString::npos )
		{
			strExt = m_strPath.substr(pos);
		}
		for ( uint32 i=0; i<strExt.size(); ++i)
		{
			strExt[i] = tolower(strExt[i]);
		}

		CGUIString strFullPath = GSystem->GetDataPath() + CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;
		if (strExt == ".ogg")
		{
			//load ogg file
			if( LoadOggFile( strFullPath ) != true)
			{
				throw CGUIException("[CGUISoundData_openal::DoLoad]: failed to load ogg file <%s>!", m_strPath.c_str());
				return -1;
			}
		}
#if GUIEX_PLATFORM_WIN32
		else if (strExt == ".wav")
		{
			//load wav file
			if( LoadWavFile( strFullPath ) != true)
			{
				throw CGUIException("[CGUISoundData_openal::DoLoad]: failed to load wav file <%s>!", m_strPath.c_str());
				return -1;
			}
		}	
#endif
		else
		{
			throw CGUIException("[CGUISoundData_openal::DoLoad]: doesn't support the sound type <%s>!", strExt.c_str());
			return -1;
		}

		//assign the buffer to this source
		alSourcei( m_nSourceId, AL_BUFFER, m_nBufferId);

		//set source position
		alSource3f( m_nSourceId,AL_POSITION, 0, 0, 0);

		//set source velocity
		alSource3f( m_nSourceId,AL_VELOCITY, 0, 0, 0);

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISoundData_openal::DoUnload()
	{
		//stop
		alSourceStop(m_nSourceId);

		//delete our source
		alDeleteSources(1,&m_nSourceId);

		//delete our buffer
		alDeleteBuffers(1,&m_nBufferId);
	}
	//------------------------------------------------------------------------------
	bool CGUISoundData_openal::SetAlBuffer( ALenum format, const ALvoid* data, ALsizei size, ALsizei freq ) const
	{
		//create 1 source
		alGenSources(1, &m_nSourceId);
		if (alGetError() != AL_NO_ERROR)
		{
			return false;
		}

		//create 1 buffer
		alGenBuffers(1, &m_nBufferId);
		if (alGetError() != AL_NO_ERROR)
		{
			return false;
		}

		//fills sample set with buffer data from alBuffer
		alBufferData( m_nBufferId, format, data, size, freq );
		if ( alGetError() != AL_NO_ERROR )
		{
			return false;
		}

		return true;
	}
	//------------------------------------------------------------------------------
#if GUIEX_PLATFORM_WIN32
	bool CGUISoundData_openal::LoadWavFile( const CGUIString& rFilename ) const
	{
		HMMIO hmmio = mmioOpenA( LPSTR( rFilename.c_str()), NULL, MMIO_ALLOCBUF | MMIO_READ);
		if (NULL == hmmio)
		{
			return false;
		}

		// check if it's a wav file
		PCMWAVEFORMAT aWaveFormat;
		MMCKINFO mmckinfo;
		MMCKINFO ckIn;
		// read next chunk
		if (0 != mmioDescend(hmmio, &mmckinfo, NULL, 0))
		{
			mmioClose(hmmio, 0);
			return false;
		}

		// check for valid wav file
		if ((mmckinfo.ckid != FOURCC_RIFF) ||
			(mmckinfo.fccType != mmioFOURCC('W','A','V','E')))
		{
			mmioClose(hmmio, 0);
			return false;
		}

		// search the fmt chunk
		ckIn.ckid = mmioFOURCC('f','m','t',' ');
		if (0 != mmioDescend(hmmio, &ckIn, &mmckinfo, MMIO_FINDCHUNK))
		{
			mmioClose(hmmio, 0);
			return false;
		}

		// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
		// if there are extra parameters at the end, we'll ignore them
		if (ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT))
		{
			mmioClose(hmmio, 0);
			return false;
		}

		// read fmt chunk into aWaveFormat
		if (mmioRead(hmmio, (HPSTR) &aWaveFormat, sizeof(aWaveFormat)) != sizeof(aWaveFormat))
		{
			mmioClose(hmmio, 0);
			return false;
		}

		// ascend the input file out of the 'fmt ' chunk
		if (0 != mmioAscend(hmmio, &ckIn, 0))
		{
			mmioClose(hmmio, 0);
			return false;
		}

		// reset the file to the beginning of data
		if (-1 == mmioSeek(hmmio, mmckinfo.dwDataOffset + sizeof(FOURCC), SEEK_SET))
		{
			mmioClose(hmmio, 0);
			return false;
		}

		// search for 'data' chunk
		ckIn.ckid = mmioFOURCC('d','a','t','a');
		if (0 != mmioDescend(hmmio, &ckIn, &mmckinfo, MMIO_FINDCHUNK))
		{
			mmioClose(hmmio, 0);
			return false;
		}

		//get data size
		uint32 nDataSize = mmckinfo.cksize;
		uint8* pDataBuffer = new uint8[nDataSize];

		//read data
		MMIOINFO mmioInfoIn;    // current status of m_hmmio
		mmioGetInfo( hmmio, &mmioInfoIn, 0);

		DWORD cT;
		for (cT = 0; cT < nDataSize; cT++)
		{
			// read next chunk if necessary
			if (mmioInfoIn.pchNext == mmioInfoIn.pchEndRead)
			{
				mmioAdvance(hmmio, &mmioInfoIn, MMIO_READ);
			}

			// copy to buffer
			*((unsigned char*)pDataBuffer+cT) = *((unsigned char*)mmioInfoIn.pchNext);
			mmioInfoIn.pchNext++;
		}
		mmioSetInfo(hmmio, &mmioInfoIn, 0);
		mmioClose(hmmio, 0);

		uint32 nFrequency = aWaveFormat.wf.nSamplesPerSec;
		uint32 ulFormat = 0;
		if (aWaveFormat.wf.nChannels == 1)
		{
			ulFormat = AL_FORMAT_MONO16;
		}
		else if (aWaveFormat.wf.nChannels == 2)
		{
			ulFormat = AL_FORMAT_STEREO16;
		}
		else if (aWaveFormat.wf.nChannels == 4)
		{
			ulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
		}
		else if (aWaveFormat.wf.nChannels == 6)
		{
			ulFormat = alGetEnumValue("AL_FORMAT_51CHN16");
		}

		//set al buffer
		bool bRet = SetAlBuffer( ulFormat, pDataBuffer, nDataSize, nFrequency );
		delete[] pDataBuffer;
		return bRet;
	}
#endif
	//------------------------------------------------------------------------------
	bool CGUISoundData_openal::LoadOggFile( const CGUIString& rFilename ) const
	{
		//open file
		FILE* pFile = fopen( rFilename.c_str(), "rb" );
		if( !pFile)
		{
			fclose( pFile );
			return false;
		}

		OggVorbis_File aVorbisFile;
		ov_callbacks sCallbacks;
		sCallbacks.read_func = ov_read_func;
		sCallbacks.seek_func = ov_seek_func;
		sCallbacks.close_func = ov_close_func;
		sCallbacks.tell_func = ov_tell_func;
		if (ov_open_callbacks(pFile, &aVorbisFile, NULL, 0, sCallbacks) != 0)
		{
			fclose( pFile );
			return false;
		}

		// Get some information about the file (Channels, Format, and Frequency)
		unsigned long ulFrequency = 0;
		unsigned long ulFormat = 0;
		unsigned long ulChannels = 0;
		if( false == GetOggVorbisInfo( &aVorbisFile, &ulFrequency, &ulFormat, &ulChannels ) )
		{
			ov_clear(&aVorbisFile);
			return false;
		}

		long ulBufferSize = static_cast<long>(ov_pcm_total(&aVorbisFile, -1));
		if( ulBufferSize <= 0 )
		{
			ov_clear(&aVorbisFile);
			return false;
		}

		// Allocate a buffer to be used to store decoded data for all Buffers
		char* pDecodeBuffer = (char*)malloc(ulBufferSize);
		if (!pDecodeBuffer)
		{
			ov_clear(&aVorbisFile);
			return false;
		}

		//decode buffer
		unsigned long ulBytesWritten = DecodeOggVorbis(&aVorbisFile, pDecodeBuffer, ulBufferSize, ulChannels);
		bool bRet = false;
		if (ulBytesWritten)
		{
			//set al buffer
			bRet = SetAlBuffer( ulFormat, pDecodeBuffer, ulBytesWritten, ulFrequency );
		}

		free( pDecodeBuffer );
		ov_clear(&aVorbisFile);
		return bRet;
	}
	//------------------------------------------------------------------------------

}
