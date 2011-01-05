/** 
* @file guisounddata_cocoa.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/


#ifndef	__KEN_GUISOUNDDATA_COCOA_20110104_H__
#define	__KEN_GUISOUNDDATA_COCOA_20110104_H__

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guisounddata.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

//============================================================================//
// class
//============================================================================//

namespace guiex
{
	/**
	* @class CGUISoundData_cocoa
	*/
	class GUIEXPORT CGUISoundData_cocoa : public CGUISoundData
	{	
	public:
		virtual ~CGUISoundData_cocoa();

	protected:
		friend class IGUISound_cocoa;
		CGUISoundData_cocoa( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );

		virtual int32 DoLoad() const;
		virtual void DoUnload();

		bool LoadWavFile( const CGUIString& rFilename ) const;
		bool LoadOggFile( const CGUIString& rFilename ) const;

		bool SetAlBuffer( ALenum format, const ALvoid* data, ALsizei size, ALsizei freq ) const;

	protected:
		CGUIString m_strPath; //sound file path

		//for openal
		mutable uint32 m_nSourceId;
		mutable uint32 m_nBufferId;
	protected:
	};
}


#endif //__KEN_GUISOUNDDATA_COCOA_20110104_H__
