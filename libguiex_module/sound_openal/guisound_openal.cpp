/** 
 * @file guisound_openal.cpp
 * @brief sound module, use openal
 * @author ken
 * @date 2006-08-31
 */

//============================================================================//
// include
//============================================================================// 
#include "guisound_openal.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisystem.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <alut.h>


namespace guiex
{
	//============================================================================//
	// function
	//============================================================================// 
	GUI_INTERFACE_IMPLEMENT(IGUISound_openal);
	//------------------------------------------------------------------------------
	const char* IGUISound_openal::StaticGetModuleName()
	{
		return "IGUISound_openal";
	}
	//------------------------------------------------------------------------------
	IGUISound_openal::IGUISound_openal()
	:IGUIInterfaceSound( StaticGetModuleName() )
	,m_pContext(NULL)
	,m_pDevice(NULL)
	{
	}
	//------------------------------------------------------------------------------
	IGUISound_openal::~IGUISound_openal()
	{
	}
	//------------------------------------------------------------------------------
	int IGUISound_openal::DoInitialize(void* )
	{
		m_pDevice = alcOpenDevice(NULL);
		if (m_pDevice == NULL)
		{
			return -1;
		}
		
		//Create context(s)
		m_pContext=alcCreateContext(m_pDevice, NULL);
		
		//Set active context
		alcMakeContextCurrent(m_pContext);
		
		// Clear Error Code
		alGetError();
		
		float vec[6];
		vec[0] = 0.0f; //forward vector x value
		vec[1] = 0.0f; //forward vector y value
		vec[2] = -1.0f; //forward vector z value
		vec[3] = 0; //up vector x value
		vec[4] = 1.0f; //up vector y value
		vec[5] = 0.0f; //up vector z value
		
		//set current listener position
		alListener3f(AL_POSITION, 0, 0, 0);
		
		//set current listener orientation
		alListenerfv(AL_ORIENTATION, vec);
		
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_openal::DoDestroy()
	{
		while(m_mapEffect.empty() == false)
		{
			UnloadEffect(m_mapEffect.begin()->first);
		}
		
		//Disable context
		alcMakeContextCurrent(NULL);
		//Release context(s)
		alcDestroyContext(m_pContext);
		//Close device
		alcCloseDevice(m_pDevice);
	}
	//------------------------------------------------------------------------------
	void IGUISound_openal::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	static CGUIString GetALErrorString(ALenum err)
	{
		switch(err)
		{
			case AL_NO_ERROR:
				return CGUIString("AL_NO_ERROR");
				break;
				
			case AL_INVALID_NAME:
				return CGUIString("AL_INVALID_NAME");
				break;
				
			case AL_INVALID_ENUM:
				return CGUIString("AL_INVALID_ENUM");
				break;
				
			case AL_INVALID_VALUE:
				return CGUIString("AL_INVALID_VALUE");
				break;
				
			case AL_INVALID_OPERATION:
				return CGUIString("AL_INVALID_OPERATION");
				break;
				
			case AL_OUT_OF_MEMORY:
				return CGUIString("AL_OUT_OF_MEMORY");
				break;
				
			default:
				return CGUIString("Unknow error for AL");
		};
	}
	//------------------------------------------------------------------------------
	int32 IGUISound_openal::LoadEffect( int32 nIdx,  const CGUIString& rFileName)
	{
		// identify file type by extension
		CGUIString strExt;
		size_t pos = rFileName.find_last_of(".");
		if( pos != CGUIString::npos )
		{
			strExt = rFileName.substr(pos);
		}
		for ( uint32 i=0; i<strExt.size(); ++i)
		{
			strExt[i] = tolower(strExt[i]);
		}
		
		SSoundData	aSoundData;
		if (strExt == ".wav")
		{
			//load wav file
			if( LoadWavFile(GSystem->GetDataPath()+rFileName, aSoundData) != true)
			{
				throw CGUIException("[IGUISound_openal::LoadEffect]: failed to load wav file <%s>!", rFileName.c_str());
				return -1;
			}
		}
		else if (strExt == ".ogg")
		{
			//load ogg file
			if( LoadOggFile(GSystem->GetDataPath()+rFileName, aSoundData) != true)
			{
				throw CGUIException("[IGUISound_openal::LoadEffect]: failed to load ogg file <%s>!", rFileName.c_str());
				return -1;
			}
		}	
		else
		{
			throw CGUIException("[IGUISound_openal::LoadEffect]: doesn't support the sound type <%s>!", strExt.c_str());
			return -1;
		}
		
		//assign the buffer to this source
		alSourcei(aSoundData.m_nSourceId, AL_BUFFER, aSoundData.m_nBufferId);
		
		//set source position
		alSource3f(aSoundData.m_nSourceId,AL_POSITION, 0, 0, 0);
		
		//set source velocity
		alSource3f(aSoundData.m_nSourceId,AL_VELOCITY, 0, 0, 0);
		
		
		//check map
		TMapSound::iterator itorFind = m_mapEffect.find(nIdx);
		if( itorFind != m_mapEffect.end())
		{
			UnloadEffect(itorFind->first);
		}
		
		m_mapEffect.insert( std::make_pair(nIdx, aSoundData));
		
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_openal::UnloadEffect( int32 nIdx )
	{
		TMapSound::iterator itorFind = m_mapEffect.find(nIdx);
		if( itorFind != m_mapEffect.end())
		{
			//delete our buffer
			alDeleteBuffers(1,&itorFind->second.m_nBufferId);
			
			//delete our source
			alDeleteSources(1,&itorFind->second.m_nSourceId);
			
			m_mapEffect.erase( itorFind );
		}
	}
	//------------------------------------------------------------------------------
	int32 IGUISound_openal::PlayEffect( int32 nIdx )
	{
		TMapSound::iterator itorFind = m_mapEffect.find(nIdx);
		if( itorFind == m_mapEffect.end())
		{
			throw CGUIException("[IGUISound_openal::PlayEffect]: failed to play sound file <%d>!", nIdx);
			return -1;
		}
		
		unsigned int alSource = itorFind->second.m_nSourceId;
		
		//tell the sound to loop continuously
		alSourcei(alSource,AL_LOOPING,AL_FALSE);
		
		//play the sound
		alSourcePlay(alSource);
		
		// To stop the sound
		//alSourceStop(alSource);
		
		return 0;
	}
	//------------------------------------------------------------------------------
	bool IGUISound_openal::LoadWavFile( const CGUIString& rFilename, SSoundData& rSoundData)
	{
		char* alBuffer;			//data for the buffer
		ALenum alFormatBuffer;	//for the buffer format
		ALsizei alFreqBuffer;	//for the frequency of the buffer
		ALsizei alBufferLen;	//the bit depth
		ALboolean alLoop;		//looped
		
		//load the wave file
		alutLoadWAVFile((ALbyte *)rFilename.c_str(),&alFormatBuffer, (void**) &alBuffer, &alBufferLen, &alFreqBuffer, &alLoop);
		if( ALUT_ERROR_NO_ERROR != alutGetError())
		{
			return false;
		}
		
		//create 1 source
		alGenSources(1, &rSoundData.m_nSourceId);
		if (alGetError() != AL_NO_ERROR)
		{
			return false;
		}
		
		//create 1 buffer
		alGenBuffers(1, &rSoundData.m_nBufferId);
		if (alGetError() != AL_NO_ERROR)
		{
			return false;
		}
		
		//fills sample set with buffer data from alBuffer
		alBufferData(rSoundData.m_nBufferId, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		if (alGetError() != AL_NO_ERROR)
		{
			return false;
		}
		
		//release the data
		alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		return true;
	}
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
	bool IGUISound_openal::LoadOggFile( const CGUIString& rFilename, SSoundData& rSoundData)
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
		
		unsigned long	ulFormat = 0;
		unsigned long	ulFrequency = pVorbisInfo->rate;
		unsigned long	ulChannels = pVorbisInfo->channels;
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
			
			// Generate some AL Buffers for streaming
			alGenBuffers( 1, &rSoundData.m_nBufferId );
			if (alGetError() != AL_NO_ERROR)
			{
				return false;
			}
			
			// Generate a Source to playback the Buffers
			alGenSources( 1, &rSoundData.m_nSourceId );
			if (alGetError() != AL_NO_ERROR)
			{
				return false;
			}
			
			// Fill all the Buffers with decoded audio data from the OggVorbis file
			alBufferData(rSoundData.m_nBufferId, ulFormat, pDecodeBuffer, ulBytesDone, ulFrequency);
			if (alGetError() != AL_NO_ERROR)
			{
				return false;
			}
			
			free( pDecodeBuffer );
		}
		else
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------
	
}//namespace guiex
