/** 
* @file guisounddata_android.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/


#ifndef	__KEN_GUISOUNDDATA_ANDROID_20110104_H__
#define	__KEN_GUISOUNDDATA_ANDROID_20110104_H__

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guisounddata.h>

//============================================================================//
// class
//============================================================================//

namespace guiex
{
	/**
	* @class CGUISoundData_android
	*/
	class GUIEXPORT CGUISoundData_android : public CGUISoundData
	{	
	public:
		virtual ~CGUISoundData_android();

	protected:
		friend class IGUISound_android;
		CGUISoundData_android( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );

		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
	};
}


#endif //__KEN_GUISOUNDDATA_ANDROID_20110510_H__
