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

	protected:
	};
}


#endif //__KEN_GUISOUNDDATA_COCOA_20110104_H__
