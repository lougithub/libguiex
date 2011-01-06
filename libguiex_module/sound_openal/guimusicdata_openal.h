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

//============================================================================//
// class
//============================================================================//

namespace guiex
{
	/**
	* @class CGUIMusicData_openal
	*/
	class GUIEXPORT CGUIMusicData_openal : public CGUIMusicData
	{	
	public:
		virtual ~CGUIMusicData_openal();

	protected:
		friend class IGUISound_openal;
		CGUIMusicData_openal( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath );

		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
	};
}


#endif //__KEN_GUIMUSICDATA_OPENAL_20110106_H__
