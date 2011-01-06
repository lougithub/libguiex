/** 
* @file guimusicdata_cocoa.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-06
*/



#ifndef	__KEN_GUIMUSICDATA_COCOA_20110106_H__
#define	__KEN_GUIMUSICDATA_COCOA_20110106_H__

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
	* @class CGUIMusicData_cocoa
	*/
	class GUIEXPORT CGUIMusicData_cocoa : public CGUIMusicData
	{	
	public:
		virtual ~CGUIMusicData_cocoa();

	protected:
		friend class IGUISound_cocoa;
		CGUIMusicData_cocoa( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );

		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
	};
}


#endif //__KEN_GUIMUSICDATA_COCOA_20110106_H__
