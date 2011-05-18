/** 
* @file guimusicdata_android.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-05-10
*/



#ifndef	__KEN_GUIMUSICDATA_ANDROID_20110510_H__
#define	__KEN_GUIMUSICDATA_ANDROID_20110510_H__

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guimusicdata.h>

//============================================================================//
// class
//============================================================================//

namespace guiex
{
	/**
	* @class CGUIMusicData_android
	*/
	class GUIEXPORT CGUIMusicData_android : public CGUIMusicData
	{	
	public:
		virtual ~CGUIMusicData_android();

	protected:
		friend class IGUISound_android;
		CGUIMusicData_android( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );

		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
	};
}


#endif //__KEN_GUIMUSICDATA_ANDROID_20110510_H__
