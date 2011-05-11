/** 
* @file guimusicdata_opensles.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-05-10
*/



#ifndef	__KEN_GUIMUSICDATA_OPENSLES_20110510_H__
#define	__KEN_GUIMUSICDATA_OPENSLES_20110510_H__

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
	* @class CGUIMusicData_opensles
	*/
	class GUIEXPORT CGUIMusicData_opensles : public CGUIMusicData
	{	
	public:
		virtual ~CGUIMusicData_opensles();

	protected:
		friend class IGUISound_opensles;
		CGUIMusicData_opensles( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );

		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
	};
}


#endif //__KEN_GUIMUSICDATA_OPENSLES_20110510_H__
