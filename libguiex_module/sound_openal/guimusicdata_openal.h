/** 
* @file guimusicdata_openal.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-06
*/



#ifndef	__KEN_GUIMUSICDATA_OPENAL_20110106_H__
#define	__KEN_GUIMUSICDATA_OPENAL_20110106_H__

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guimusicdata.h>

#ifndef OV_EXCLUDE_STATIC_CALLBACKS
#define OV_EXCLUDE_STATIC_CALLBACKS
#endif
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>


#if GUIEX_PLATFORM_MAC
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <al.h>
#include <alc.h>
#endif

//============================================================================//
// class
//============================================================================//
#define GUI_MUSIC_NUMBUFFERS              (4)

namespace guiex
{
	/**
	* @class CGUIMusicData_openal
	*/
	class GUIEXPORT CGUIMusicData_openal : public CGUIMusicData
	{	
	public:
		bool Update();

	protected:
		friend class IGUISound_openal;
		CGUIMusicData_openal( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual ~CGUIMusicData_openal();

		virtual int32 DoLoad();
		virtual void DoUnload();

		bool LoadOggFile( const CGUIString& rFilename ) const;

	protected:
		CGUIString m_strPath; //sound file path

		//for openal
		mutable uint32 m_nSourceId;
		mutable ALuint m_nBuffers[GUI_MUSIC_NUMBUFFERS];

		mutable char* m_pDecodeBuffer;
		mutable unsigned long m_nBufferSize;
		mutable unsigned long m_nChannels;
		mutable unsigned long m_nFrequency;
		mutable unsigned long m_nFormat;
		mutable OggVorbis_File m_aVorbisFile;
	};
}


#endif //__KEN_GUIMUSICDATA_OPENAL_20110106_H__
