/** 
* @file guioggfunc.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-07
*/

//============================================================================//
// include
//============================================================================//
#include "guioggfunc.h"
#include <al.h>
#include <alc.h>
#include <algorithm>

namespace guiex
{
	//------------------------------------------------------------------------------
	size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		return fread(ptr, size, nmemb, (FILE*)datasource);
	}
	//------------------------------------------------------------------------------
	int ov_seek_func(void *datasource, ogg_int64_t offset, int whence)
	{
		return fseek((FILE*)datasource, (long)offset, whence);
	}
	//------------------------------------------------------------------------------
	int ov_close_func(void *datasource)
	{
		return fclose((FILE*)datasource);
	}
	//------------------------------------------------------------------------------
	long ov_tell_func(void *datasource)
	{
		return ftell((FILE*)datasource);
	}
	//------------------------------------------------------------------------------
	bool GetOggVorbisInfo( OggVorbis_File *psOggVorbisFile, unsigned long* pFrequency, unsigned long* pFormat, unsigned long* pChannels, unsigned long* pBufferSize )
	{
		vorbis_info*pVorbisInfo = ov_info(psOggVorbisFile, -1);
		if( !pVorbisInfo)
		{
			return false;
		}

		*pFrequency = pVorbisInfo->rate;
		*pFormat = 0;
		*pChannels = pVorbisInfo->channels;

		if (*pChannels == 1)
		{
			*pFormat = AL_FORMAT_MONO16;

			if( pBufferSize )
			{
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				*pBufferSize = *pFrequency >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				*pBufferSize -= (*pBufferSize % 2);
			}
		}
		else if (*pChannels == 2)
		{
			*pFormat = AL_FORMAT_STEREO16;
			if( pBufferSize )
			{
				// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
				*pBufferSize = *pFrequency;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				*pBufferSize -= (*pBufferSize % 4);
			}
		}
		else if (*pChannels == 4)
		{
			*pFormat = alGetEnumValue("AL_FORMAT_QUAD16");

			if( pBufferSize )
			{
				// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
				*pBufferSize = *pFrequency * 2;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				*pBufferSize -= (*pBufferSize % 8);
			}
		}
		else if (*pChannels == 6)
		{
			*pFormat = alGetEnumValue("AL_FORMAT_51CHN16");

			if( pBufferSize )
			{
				// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
				*pBufferSize = *pFrequency * 3;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				*pBufferSize -= (*pBufferSize % 12);
			}
		}
		else
		{
			return false;
		}

		return true;
	}
	//------------------------------------------------------------------------------
	unsigned long DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels )
	{
		int current_section;
		long lDecodeSize;
		unsigned long ulSamples;
		short *pSamples;

		unsigned long ulBytesDone = 0;
		while (1)
		{
			lDecodeSize = ov_read(psOggVorbisFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
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

		// Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
		// however 6-Channels files need to be re-ordered
		if (ulChannels == 6)
		{		
			pSamples = (short*)pDecodeBuffer;
			for (ulSamples = 0; ulSamples < (ulBufferSize>>1); ulSamples+=6)
			{
				// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
				// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
				std::swap(pSamples[ulSamples+1], pSamples[ulSamples+2]);
				std::swap(pSamples[ulSamples+3], pSamples[ulSamples+5]);
				std::swap(pSamples[ulSamples+4], pSamples[ulSamples+5]);
			}
		}

		return ulBytesDone;
	}
	//------------------------------------------------------------------------------
}
