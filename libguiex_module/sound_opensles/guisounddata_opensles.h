/** 
* @file guisounddata_opensles.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/


#ifndef	__KEN_GUISOUNDDATA_OPENSLES_20110104_H__
#define	__KEN_GUISOUNDDATA_OPENSLES_20110104_H__

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
	* @class CGUISoundData_opensles
	*/
	class GUIEXPORT CGUISoundData_opensles : public CGUISoundData
	{	
	public:
		virtual ~CGUISoundData_opensles();

	protected:
		friend class IGUISound_opensles;
		CGUISoundData_opensles( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );

		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
	};
}


#endif //__KEN_GUISOUNDDATA_OPENSLES_20110510_H__
