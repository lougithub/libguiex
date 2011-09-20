/** 
* @file guimusicdata_openal.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-06
*/

//============================================================================//
// include
//============================================================================//
#include "guimusicdata_openal.h"
#include "guioggfunc.h"
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiscenemanager.h>
#include <libguiex_core/guiexception.h>

#if GUIEX_PLATFORM_LINUX
#include <ctype.h>
#endif

//============================================================================//
// function
//============================================================================//
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIMusicData_openal::CGUIMusicData_openal( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUIMusicData( rName, rSceneName )
		,m_strPath( rPath )
		,m_pDecodeBuffer( NULL )
		,m_nBufferSize( 0 )
		,m_nChannels( 0 )
		,m_nFormat( 0 )
		,m_nFrequency( 0 )
	{
	}
	//------------------------------------------------------------------------------
	CGUIMusicData_openal::~CGUIMusicData_openal()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIMusicData_openal::DoLoad()
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

		CGUIString	strScenePath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;
		CGUIString	strFullPath;
		GSystem->GenerateFullPath( strScenePath, strFullPath );
		if (strExt == ".ogg")
		{
			//load ogg file
			if( LoadOggFile( strFullPath ) != true)
			{
				GUI_THROW( GUI_FORMAT("[CGUISoundData_openal::DoLoad]: failed to load ogg file <%s>!", m_strPath.c_str()));
				return -1;
			}
		}
		else
		{
			GUI_THROW( GUI_FORMAT("[CGUISoundData_openal::DoLoad]: doesn't support the sound type <%s>!", strExt.c_str()));
			return -1;
		}

		//assign the buffer to this source
		//alSourcei( m_nSourceId, AL_BUFFER, m_nBufferId);

		//set source position
		alSource3f( m_nSourceId,AL_POSITION, 0, 0, 0);

		//set source velocity
		alSource3f( m_nSourceId,AL_VELOCITY, 0, 0, 0);
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIMusicData_openal::DoUnload()
	{
		//stop
		alSourceStop(m_nSourceId);

		//clear the Queue
		alSourcei(m_nSourceId, AL_BUFFER, 0);

		//delete our source
		alDeleteSources(1,&m_nSourceId);

		//delete our buffer
		alDeleteBuffers( GUI_MUSIC_NUMBUFFERS, m_nBuffers );

		if( m_pDecodeBuffer )
		{
			free( m_pDecodeBuffer );
			m_pDecodeBuffer = NULL;
		}
		m_nBufferSize = 0;
		m_nChannels = 0;
		m_nFormat = 0;
		m_nFrequency = 0;

		ov_clear(&m_aVorbisFile);
	}
	//------------------------------------------------------------------------------
	bool CGUIMusicData_openal::LoadOggFile( const CGUIString& rFilename ) const
	{
		//open file
		FILE *pFile = fopen( rFilename.c_str(), "rb" );
		if( !pFile)
		{
			return false;
		}

		ov_callbacks sCallbacks;
		sCallbacks.read_func = ov_read_func;
		sCallbacks.seek_func = ov_seek_func;
		sCallbacks.close_func = ov_close_func;
		sCallbacks.tell_func = ov_tell_func;
		if (ov_open_callbacks(pFile, &m_aVorbisFile, NULL, 0, sCallbacks) != 0)
		{
			fclose( pFile );
			return false;
		}

		// Get some information about the file (Channels, Format, and Frequency)
		if( false == GetOggVorbisInfo( &m_aVorbisFile, &m_nFrequency, &m_nFormat, &m_nFormat, &m_nBufferSize ) )
		{
			ov_clear(&m_aVorbisFile);
			return false;
		}

		// Allocate a buffer to be used to store decoded data for all Buffers
		m_pDecodeBuffer = (char*)malloc(m_nBufferSize);
		if ( !m_pDecodeBuffer )
		{
			ov_clear(&m_aVorbisFile);
			return false;
		}

		// Generate a Source to playback the Buffers
		alGenSources(1, &m_nSourceId);
		if (alGetError() != AL_NO_ERROR)
		{
			return false;
		}

		// Generate some AL Buffers for streaming
		alGenBuffers( GUI_MUSIC_NUMBUFFERS, m_nBuffers );
		if (alGetError() != AL_NO_ERROR)
		{
			return false;
		}

		// Fill all the Buffers with decoded audio data from the OggVorbis file
		for (int iLoop = 0; iLoop < GUI_MUSIC_NUMBUFFERS; iLoop++)
		{
			unsigned long ulBytesWritten = DecodeOggVorbis(&m_aVorbisFile, m_pDecodeBuffer, m_nBufferSize, m_nChannels);
			if (ulBytesWritten)
			{
				alBufferData(m_nBuffers[iLoop], m_nFormat, m_pDecodeBuffer, ulBytesWritten, m_nFrequency);
				alSourceQueueBuffers(m_nSourceId, 1, &m_nBuffers[iLoop]);
			}
		}

		return true;
	}
	//------------------------------------------------------------------------------
	bool CGUIMusicData_openal::Update()
	{
		// Request the number of OpenAL Buffers have been processed (played) on the Source
		ALint iBuffersProcessed = 0;
		alGetSourcei(m_nSourceId, AL_BUFFERS_PROCESSED, &iBuffersProcessed);

		// For each processed buffer, remove it from the Source Queue, read next chunk of audio
		// data from disk, fill buffer with new data, and add it to the Source Queue
		while (iBuffersProcessed)
		{
			// Remove the Buffer from the Queue.  (uiBuffer contains the Buffer ID for the unqueued Buffer)
			ALuint uiBuffer = 0;
			alSourceUnqueueBuffers(m_nSourceId, 1, &uiBuffer);

			// Read more audio data (if there is any)
			unsigned long ulBytesWritten = DecodeOggVorbis( &m_aVorbisFile, m_pDecodeBuffer, m_nBufferSize, m_nChannels );
			if (ulBytesWritten)
			{
				alBufferData(uiBuffer, m_nFormat, m_pDecodeBuffer, ulBytesWritten, m_nFrequency);
				alSourceQueueBuffers(m_nSourceId, 1, &uiBuffer);
			}

			iBuffersProcessed--;
		}

		// Check the status of the Source.  If it is not playing, then playback was completed,
		// or the Source was starved of audio data, and needs to be restarted.
		ALint iState = 0;
		alGetSourcei(m_nSourceId, AL_SOURCE_STATE, &iState);
		if (iState == AL_PAUSED)
		{
		}
		else if (iState != AL_PLAYING)
		{
			// If there are Buffers in the Source Queue then the Source was starved of audio
			// data, so needs to be restarted (because there is more audio data to play)
			ALint iQueuedBuffers = 0;
			alGetSourcei(m_nSourceId, AL_BUFFERS_QUEUED, &iQueuedBuffers);
			if (iQueuedBuffers)
			{
				alSourcePlay(m_nSourceId);
			}
			else
			{
				// Finished playing
				return false;
			}
		}

		return true;
	}
	//------------------------------------------------------------------------------

}
