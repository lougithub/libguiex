/** 
* @file guimusicdata_dummy.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-06
*/



#ifndef	__KEN_GUIMUSICDATA_DUMMY_20110106_H__
#define	__KEN_GUIMUSICDATA_DUMMY_20110106_H__

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
	* @class CGUIMusicData_dummy
	*/
	class GUIEXPORT CGUIMusicData_dummy : public CGUIMusicData
	{	
	public:
		virtual ~CGUIMusicData_dummy();

	protected:
		friend class IGUISound_dummy;
		CGUIMusicData_dummy( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );

		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
	};
}


#endif //__KEN_GUIMUSICDATA_DUMMY_20110106_H__
