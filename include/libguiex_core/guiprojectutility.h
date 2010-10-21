/** 
* @file GuiexProjectutility.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/






#ifndef	__KEN_GUIEXPROJECTUTILITY_20091022_H__
#define __KEN_GUIEXPROJECTUTILITY_20091022_H__





//============================================================================//
// include
//============================================================================//
#include "guibase.h"




//============================================================================//
// declare
//============================================================================//

namespace guiex
{
	class CGUIWidget;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIProjectUtility
	{
	public:
		static int32 LoadResource(const CGUIString& strProjectName);
		static int32 UnloadResource(const CGUIString& strProjectName);

		static int32 LoadPages( const CGUIString& strProjectName);
	};
}		//namespace guiex


#endif //__KEN_GUIEXPROJECTUTILITY_20091022_H__

