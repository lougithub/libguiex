/** 
* @file guisounddata_cocoa.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/

//============================================================================//
// include
//============================================================================//
#include "guisounddata_cocoa.h"
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiscenemanager.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	static size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		return fread(ptr, size, nmemb, (FILE*)datasource);
	}
	//------------------------------------------------------------------------------
	static int ov_seek_func(void *datasource, ogg_int64_t offset, int whence)
	{
		return fseek((FILE*)datasource, (long)offset, whence);
	}
	//------------------------------------------------------------------------------
	static int ov_close_func(void *datasource)
	{
		return fclose((FILE*)datasource);
	}
	//------------------------------------------------------------------------------
	static long ov_tell_func(void *datasource)
	{
		return ftell((FILE*)datasource);
	}
	//------------------------------------------------------------------------------





	//------------------------------------------------------------------------------
	CGUISoundData_cocoa::CGUISoundData_cocoa( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUISoundData( rName, rSceneName )
		 ,m_strPath( rPath )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData_cocoa::~CGUISoundData_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundData_cocoa::DoLoad() const
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
		if (strExt == ".wav")
		{
			//load wav file
			if( LoadWavFile( strFullPath ) != true)
			{
				throw CGUIException("[CGUISoundData_cocoa::DoLoad]: failed to load wav file <%s>!", m_strPath.c_str());
				return -1;
			}
		}
		else if (strExt == ".ogg")
		{
			//load ogg file
			if( LoadOggFile( strFullPath ) != true)
			{
				throw CGUIException("[CGUISoundData_cocoa::DoLoad]: failed to load ogg file <%s>!", m_strPath.c_str());
				return -1;
			}
		}	
		else
		{
			throw CGUIException("[CGUISoundData_cocoa::DoLoad]: doesn't support the sound type <%s>!", strExt.c_str());
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
	void CGUISoundData_cocoa::DoUnload()
	{
		//delete our buffer
		alDeleteBuffers(1,&m_nBufferId);

		//delete our source
		alDeleteSources(1,&m_nSourceId);
	}
	//------------------------------------------------------------------------------
	bool CGUISoundData_cocoa::SetAlBuffer( ALenum format, const ALvoid* data, ALsizei size, ALsizei freq ) const
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
	bool CGUISoundData_cocoa::LoadWavFile( const CGUIString& rFilename ) const
	{
		return false;
	}
	//------------------------------------------------------------------------------
	bool CGUISoundData_cocoa::LoadOggFile( const CGUIString& rFilename ) const
	{
		//open file
		FILE* pFile = fopen( rFilename.c_str(), "rb" );
		if( !pFile)
		{
			return false;
		}

		OggVorbis_File	aVorbisFile;
		ov_callbacks		sCallbacks;
		sCallbacks.read_func = ov_read_func;
		sCallbacks.seek_func = ov_seek_func;
		sCallbacks.close_func = ov_close_func;
		sCallbacks.tell_func = ov_tell_func;
		if (ov_open_callbacks(pFile, &aVorbisFile, NULL, 0, sCallbacks) != 0)
		{
			fclose(pFile);
			return false;
		}

		// Get some information about the file (Channels, Format, and Frequency)
		vorbis_info*pVorbisInfo = ov_info(&aVorbisFile, -1);
		if( !pVorbisInfo)
		{
			fclose(pFile);
			return false;
		}

		unsigned long	ulFrequency = pVorbisInfo->rate;
		unsigned long	ulFormat = 0;
		if (pVorbisInfo->channels == 1)
		{
			ulFormat = AL_FORMAT_MONO16;
		}
		else if (pVorbisInfo->channels == 2)
		{
			ulFormat = AL_FORMAT_STEREO16;
		}
		else if (pVorbisInfo->channels == 4)
		{
			ulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
		}
		else if (pVorbisInfo->channels == 6)
		{
			ulFormat = alGetEnumValue("AL_FORMAT_51CHN16");
		}

		if (ulFormat != 0)
		{
			long	ulBufferSize = static_cast<long>(ov_pcm_total(&aVorbisFile, -1));
			if( ulBufferSize <= 0 )
			{
				return false;
			}

			// Allocate a buffer to be used to store decoded data for all Buffers
			char*	pDecodeBuffer = (char*)malloc(ulBufferSize);
			if (!pDecodeBuffer)
			{
				return false;
			}

			//decode buffer
			int current_section;
			long lDecodeSize = 0;
			long ulBytesDone = 0;
			while(1)
			{
				lDecodeSize = ov_read( &aVorbisFile, (char*)pDecodeBuffer+ulBytesDone, ulBufferSize-ulBytesDone, 0, 2, 1,  &current_section);
				if (lDecodeSize > 0)
				{
					ulBytesDone += lDecodeSize;
					if (ulBytesDone >= ulBufferSize)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			if( ulBytesDone <= 0 )
			{
				return false;
			}

			//set al buffer
			bool bRet = SetAlBuffer( ulFormat, pDecodeBuffer, ulBytesDone, ulFrequency );
			free( pDecodeBuffer );

			return bRet;
		}
		else
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------
}
