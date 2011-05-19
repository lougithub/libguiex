/** 
* @file guimusicdata.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/


#ifndef	__KEN_GUIMUSICDATA_20110106_H__
#define	__KEN_GUIMUSICDATA_20110106_H__

//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guistring.h"
#include "guiresource.h"


//============================================================================//
// declare
//============================================================================//



//============================================================================//
// class
//============================================================================//

namespace guiex
{
	/**
	* @class CGUIMusicData
	*/
	class GUIEXPORT CGUIMusicData : public CGUIResource
	{	
	public:
		virtual ~CGUIMusicData();

	protected:
		CGUIMusicData( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		friend class CGUISoundManager;

	protected:
		//disable =
		CGUIMusicData( const CGUIMusicData&  );
		const CGUIMusicData& operator=(const CGUIMusicData& );
	};
}


#endif //__KEN_GUIMUSICDATA_20110106_H__
