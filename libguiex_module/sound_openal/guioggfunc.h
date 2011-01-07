/** 
* @file guioggfunc.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-07
*/

#ifndef	__KEN_GUIOGGFUNC_20110107_H__
#define	__KEN_GUIOGGFUNC_20110107_H__

//============================================================================//
// include
//============================================================================//
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>


//============================================================================//
// declare
//============================================================================//

namespace guiex
{
	extern bool GetOggVorbisInfo( OggVorbis_File *psOggVorbisFile, unsigned long* pFrequency, unsigned long* pFormat, unsigned long* pChannels, unsigned long* pBufferSize=NULL );
	extern unsigned long DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels);

	extern size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource);
	extern int ov_seek_func(void *datasource, ogg_int64_t offset, int whence);
	extern int ov_close_func(void *datasource);
	extern long ov_tell_func(void *datasource);
}

#endif //__KEN_GUIOGGFUNC_20110107_H__
