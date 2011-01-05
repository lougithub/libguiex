/** 
* @file guisounddata_openal.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/


#ifndef	__KEN_GUISOUNDDATA_OPENAL_20110104_H__
#define	__KEN_GUISOUNDDATA_OPENAL_20110104_H__

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guisounddata.h>
#include <al.h>
#include <alc.h>

//============================================================================//
// class
//============================================================================//
namespace guiex
{
	/**
	* @class CGUISoundData_openal
	*/
	class GUIEXPORT CGUISoundData_openal : public CGUISoundData
	{	
	protected:
		friend class IGUISound_openal;
		CGUISoundData_openal( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );
		virtual ~CGUISoundData_openal();

		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
		bool LoadWavFile( const CGUIString& rFilename ) const;
		bool LoadOggFile( const CGUIString& rFilename ) const;

		bool SetAlBuffer( ALenum format, const ALvoid* data, ALsizei size, ALsizei freq ) const;

	protected:
		CGUIString m_strPath; //sound file path

		//for openal
		mutable uint32 m_nSourceId;
		mutable uint32 m_nBufferId;
	};
}


#endif //__KEN_GUISOUNDDATA_OPENAL_20110104_H__
